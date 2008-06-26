/* Copyright 2002-2004 Justin Erenkrantz and Greg Stein
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ----
 *
 * For the OpenSSL thread-safety locking code:
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Originally developed by Aaron Bannert and Justin Erenkrantz, eBuilt.
 */

#include <apr_pools.h>
#include <apr_network_io.h>
#include <apr_portable.h>
#include <apr_strings.h>

#include "serf.h"
#include "serf_bucket_util.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pkcs12.h>

/*#define SSL_VERBOSE*/

/*
 * Here's an overview of the SSL bucket's relationship to OpenSSL and serf.
 *
 * HTTP request:  SSLENCRYPT(REQUEST)
 *   [context.c reads from SSLENCRYPT and writes out to the socket]
 * HTTP response: RESPONSE(SSLDECRYPT(SOCKET))
 *   [handler function reads from RESPONSE which in turn reads from SSLDECRYPT]
 *
 * HTTP request read call path:
 *
 * write_to_connection
 *  |- serf_bucket_read on SSLENCRYPT
 *    |- serf_ssl_read
 *      |- serf_databuf_read
 *        |- common_databuf_prep
 *          |- ssl_encrypt
 *            |- 1. Try to read pending encrypted data; If available, return.
 *            |- 2. Try to read from ctx->stream [REQUEST bucket]
 *            |- 3. Call SSL_write with read data
 *              |- ...
 *                |- bio_bucket_write with encrypted data
 *                  |- store in sink
 *            |- 4. If successful, read pending encrypted data and return.
 *            |- 5. If fails, place read data back in ctx->stream
 *
 * HTTP response read call path:
 *
 * read_from_connection
 *  |- acceptor
 *  |- handler
 *    |- ...
 *      |- serf_bucket_read(SSLDECRYPT)
 *        |- serf_ssl_read
 *          |- serf_databuf_read
 *            |- ssl_decrypt
 *              |- 1. SSL_read() for pending decrypted data; if any, return.
 *              |- 2. Try to read from ctx->stream [SOCKET bucket]
 *              |- 3. Append data to ssl_ctx->source
 *              |- 4. Call SSL_read()
 *                |- ...
 *                  |- bio_bucket_read
 *                    |- read data from ssl_ctx->source
 *              |- If data read, return it.
 *              |- If an error, set the STATUS value and return.
 *
 */

typedef struct bucket_list {
    serf_bucket_t *bucket;
    struct bucket_list *next;
} bucket_list_t;

typedef struct {
    /* Helper to read data. Wraps stream. */
    serf_databuf_t databuf;

    /* Our source for more data. */
    serf_bucket_t *stream;

    /* The next set of buckets */
    bucket_list_t *stream_next;

    /* The status of the last thing we read. */
    apr_status_t status;

    /* Data we've read but not processed. */
    serf_bucket_t *pending;
} serf_ssl_stream_t;

struct serf_ssl_context_t {
    /* How many open buckets refer to this context. */
    int refcount;

    /* The pool that this context uses. */
    apr_pool_t *pool;

    /* The allocator associated with the above pool. */
    serf_bucket_alloc_t *allocator;

    /* Internal OpenSSL parameters */
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *bio;

    serf_ssl_stream_t encrypt;
    serf_ssl_stream_t decrypt;

    /* Client cert callbacks */
    serf_ssl_need_client_cert_t cert_callback;
    void *cert_userdata;
    apr_pool_t *cert_cache_pool;
    const char *cert_file_success;

    /* Client cert PW callbacks */
    serf_ssl_need_cert_password_t cert_pw_callback;
    void *cert_pw_userdata;
    apr_pool_t *cert_pw_cache_pool;
    const char *cert_pw_success;

    const char *cert_path;

    X509 *cached_cert;
    EVP_PKEY *cached_cert_pw;
};

typedef struct {
    /* The bucket-independent ssl context that this bucket is associated with */
    serf_ssl_context_t *ssl_ctx;

    /* Pointer to the 'right' databuf. */
    serf_databuf_t *databuf;

    /* Pointer to our stream, so we can find it later. */
    serf_bucket_t **our_stream;
} ssl_context_t;


/* Returns the amount read. */
static int bio_bucket_read(BIO *bio, char *in, int inlen)
{
    serf_ssl_context_t *ctx = bio->ptr;
    const char *data;
    apr_status_t status;
    apr_size_t len;

#ifdef SSL_VERBOSE
    printf("bio_bucket_read called for %d bytes\n", inlen);
#endif

    BIO_clear_retry_flags(bio);

    status = serf_bucket_read(ctx->decrypt.pending, inlen, &data, &len);

    ctx->decrypt.status = status;
#ifdef SSL_VERBOSE
    printf("bio_bucket_read received %d bytes (%d)\n", len, status);
#endif

    if (!SERF_BUCKET_READ_ERROR(status)) {
        /* Oh suck. */
        if (len) {
            memcpy(in, data, len);
            return len;
        }
        if (APR_STATUS_IS_EOF(status)) {
            BIO_set_retry_read(bio);
            return -1;
        }
    }

    return -1;
}

/* Returns the amount written. */
static int bio_bucket_write(BIO *bio, const char *in, int inl)
{
    serf_ssl_context_t *ctx = bio->ptr;
    serf_bucket_t *tmp;

#ifdef SSL_VERBOSE
    printf("bio_bucket_write called for %d bytes\n", inl);
#endif
    BIO_clear_retry_flags(bio);

    tmp = serf_bucket_simple_copy_create(in, inl,
                                         ctx->encrypt.pending->allocator);

    serf_bucket_aggregate_append(ctx->encrypt.pending, tmp);

    return inl;
}

/* Returns the amount read. */
static int bio_file_read(BIO *bio, char *in, int inlen)
{
    apr_file_t *file = bio->ptr;
    const char *data;
    apr_status_t status;
    apr_size_t len;

    BIO_clear_retry_flags(bio);

    len = inlen;
    status = apr_file_read(file, in, &len);

    if (!SERF_BUCKET_READ_ERROR(status)) {
        /* Oh suck. */
        if (APR_STATUS_IS_EOF(status)) {
            BIO_set_retry_read(bio);
            return -1;
        } else {
            return len;
        }
    }

    return -1;
}

/* Returns the amount written. */
static int bio_file_write(BIO *bio, const char *in, int inl)
{
    apr_file_t *file = bio->ptr;
    apr_size_t nbytes;

    BIO_clear_retry_flags(bio);

    nbytes = inl;
    apr_file_write(file, in, &nbytes);

    return nbytes;
}

static int bio_file_gets(BIO *bio, char *in, int inlen)
{
    return bio_file_read(bio, in, inlen);
}

static int bio_bucket_create(BIO *bio)
{
    bio->shutdown = 1;
    bio->init = 1;
    bio->num = -1;
    bio->ptr = NULL;

    return 1;
}

static int bio_bucket_destroy(BIO *bio)
{
    /* Did we already free this? */
    if (bio == NULL) {
        return 0;
    }

    return 1;
}

static long bio_bucket_ctrl(BIO *bio, int cmd, long num, void *ptr)
{
    long ret = 1;

    switch (cmd) {
    default:
        /* abort(); */
        break;
    case BIO_CTRL_FLUSH:
        /* At this point we can't force a flush. */
        break;
    case BIO_CTRL_PUSH:
    case BIO_CTRL_POP:
        ret = 0;
        break;
    }
    return ret;
}

static BIO_METHOD bio_bucket_method = {
    BIO_TYPE_MEM,
    "Serf SSL encryption and decryption buckets",
    bio_bucket_write,
    bio_bucket_read,
    NULL,                        /* Is this called? */
    NULL,                        /* Is this called? */
    bio_bucket_ctrl,
    bio_bucket_create,
    bio_bucket_destroy,
#ifdef OPENSSL_VERSION_NUMBER
    NULL /* sslc does not have the callback_ctrl field */
#endif
};

static BIO_METHOD bio_file_method = {
    BIO_TYPE_FILE,
    "Wrapper around APR file structures",
    bio_file_write,
    bio_file_read,
    NULL,                        /* Is this called? */
    bio_file_gets,               /* Is this called? */
    bio_bucket_ctrl,
    bio_bucket_create,
    bio_bucket_destroy,
#ifdef OPENSSL_VERSION_NUMBER
    NULL /* sslc does not have the callback_ctrl field */
#endif
};

/* This function reads an encrypted stream and returns the decrypted stream. */
static apr_status_t ssl_decrypt(void *baton, apr_size_t bufsize,
                                char *buf, apr_size_t *len)
{
    serf_ssl_context_t *ctx = baton;
    apr_size_t priv_len;
    apr_status_t status;
    const char *data;
    int ssl_len;

    /* Is there some data waiting to be read? */
    ssl_len = SSL_read(ctx->ssl, buf, bufsize);
    if (ssl_len > 0) {
#ifdef SSL_VERBOSE
        printf("ssl_decrypt: %d bytes (%d); status: %d; flags: %d\n",
               ssl_len, bufsize, ctx->decrypt.status,
               BIO_get_retry_flags(ctx->bio));
#endif
        *len = ssl_len;
        return APR_SUCCESS;
    }

    status = serf_bucket_read(ctx->decrypt.stream, bufsize, &data, &priv_len);

    if (!SERF_BUCKET_READ_ERROR(status) && priv_len) {
        serf_bucket_t *tmp;

#ifdef SSL_VERBOSE
        printf("ssl_decrypt: read %d bytes (%d); status: %d\n", priv_len,
               bufsize, status);
#endif

        tmp = serf_bucket_simple_copy_create(data, priv_len,
                                             ctx->decrypt.pending->allocator);

        serf_bucket_aggregate_append(ctx->decrypt.pending, tmp);

        ssl_len = SSL_read(ctx->ssl, buf, bufsize);
        if (ssl_len == -1) {
            int ssl_err;

            ssl_err = SSL_get_error(ctx->ssl, ssl_len);
            switch (ssl_err) {
            case SSL_ERROR_SYSCALL:
                *len = 0;
                status = ctx->decrypt.status;
                break;
            case SSL_ERROR_WANT_READ:
                *len = 0;
                status = APR_EAGAIN;
                break;
            default:
                *len = 0;
                status = APR_EGENERAL;
                break;
            }
        }
        else {
            *len = ssl_len;
        }
    }
    else {
        *len = 0;
    }
#ifdef SSL_VERBOSE
    printf("ssl_decrypt: %d %d %d\n", status, *len,
           BIO_get_retry_flags(ctx->bio));
#endif
    return status;
}

/* This function reads a decrypted stream and returns an encrypted stream. */
static apr_status_t ssl_encrypt(void *baton, apr_size_t bufsize,
                                char *buf, apr_size_t *len)
{
    const char *data;
    serf_ssl_context_t *ctx = baton;
    apr_status_t status;

    /* Try to read unread data first. */
    status = serf_bucket_read(ctx->encrypt.pending, bufsize, &data, len);
    if (SERF_BUCKET_READ_ERROR(status)) {
        return status;
    }

    /* Aha, we read something.  Return that now. */
    if (*len) {
        memcpy(buf, data, *len);
        if (APR_STATUS_IS_EOF(status)) {
            status = APR_SUCCESS;
        }
#ifdef SSL_VERBOSE
        printf("ssl_encrypt: %d %d %d (quick read)\n", status, *len,
               BIO_get_retry_flags(ctx->bio));
#endif
        return status;
    }

    if (BIO_should_retry(ctx->bio) && BIO_should_write(ctx->bio)) {
#ifdef SSL_VERBOSE
        printf("ssl_encrypt: %d %d %d (should write exit)\n", status, *len,
               BIO_get_retry_flags(ctx->bio));
#endif
        return APR_EAGAIN;
    }

    /* Oh well, read from our stream now. */
    if (!APR_STATUS_IS_EOF(ctx->encrypt.status)) {
        status = serf_bucket_read(ctx->encrypt.stream, bufsize, &data, len);
    }
    else {
        *len = 0;
        status = APR_EOF;
    }

    if (!SERF_BUCKET_READ_ERROR(status) && *len) {
        int ssl_len;

#ifdef SSL_VERBOSE
        printf("ssl_encrypt: bucket read %d bytes; status %d\n", *len, status);
#endif
        ctx->encrypt.status = status;

        ssl_len = SSL_write(ctx->ssl, data, *len);
#ifdef SSL_VERBOSE
        printf("ssl_encrypt: SSL write: %d\n", ssl_len);
#endif
        if (ssl_len == -1) {
            int ssl_err;
            serf_bucket_t *tmp;

            /* Ah, bugger. We need to put that data back. */
            if (!SERF_BUCKET_IS_AGGREGATE(ctx->encrypt.stream)) {
                tmp = serf_bucket_aggregate_create(ctx->encrypt.stream->allocator);
                serf_bucket_aggregate_append(tmp, ctx->encrypt.stream);
                ctx->encrypt.stream = tmp;
            }

            tmp = serf_bucket_simple_copy_create(data, *len,
                                             ctx->encrypt.stream->allocator);

            serf_bucket_aggregate_prepend(ctx->encrypt.stream, tmp);

            ssl_err = SSL_get_error(ctx->ssl, ssl_len);
            if (ssl_err == SSL_ERROR_SYSCALL) {
                status = ctx->encrypt.status;
                if (SERF_BUCKET_READ_ERROR(status)) {
                    return status;
                }
            }
            else {
                /* Oh, no. */
                if (ssl_err == SSL_ERROR_WANT_READ) {
                    status = APR_EAGAIN;
                }
                else {
                    status = APR_EGENERAL;
                }
            }
            *len = 0;
        }
        else {
            apr_status_t agg_status;

            /* We read something! */
            agg_status = serf_bucket_read(ctx->encrypt.pending, bufsize,
                                          &data, len);

            memcpy(buf, data, *len);

            if (APR_STATUS_IS_EOF(status) && !APR_STATUS_IS_EOF(agg_status)) {
                status = agg_status;
            }
        }
    }

#ifdef SSL_VERBOSE
    printf("ssl_encrypt finished: %d %d %d\n", status, *len,
           BIO_get_retry_flags(ctx->bio));
#endif
    return status;
}

#if APR_HAS_THREADS
apr_pool_t *ssl_pool;
apr_thread_mutex_t **ssl_locks;

typedef struct CRYPTO_dynlock_value {
    apr_thread_mutex_t *lock;
} CRYPTO_dynlock_value;

static CRYPTO_dynlock_value *ssl_dyn_create(const char* file, int line)
{
    CRYPTO_dynlock_value *l;
    apr_status_t rv;

    l = apr_palloc(ssl_pool, sizeof(CRYPTO_dynlock_value));
    rv = apr_thread_mutex_create(&l->lock, APR_THREAD_MUTEX_DEFAULT, ssl_pool);
    if (rv != APR_SUCCESS) {
        /* FIXME: return error here */
    }
    return l;
}

static void ssl_dyn_lock(int mode, CRYPTO_dynlock_value *l, const char *file,
                         int line)
{
    if (mode & CRYPTO_LOCK) {
        apr_thread_mutex_lock(l->lock);
    }
    else if (mode & CRYPTO_UNLOCK) {
        apr_thread_mutex_unlock(l->lock);
    }
}

static void ssl_dyn_destroy(CRYPTO_dynlock_value *l, const char *file,
                            int line)
{
    apr_thread_mutex_destroy(l->lock);
}

static void ssl_lock(int mode, int n, const char *file, int line)
{
    if (mode & CRYPTO_LOCK) {
        apr_thread_mutex_lock(ssl_locks[n]);
    }
    else if (mode & CRYPTO_UNLOCK) {
        apr_thread_mutex_unlock(ssl_locks[n]);
    }
}

static unsigned long ssl_id(void)
{
    /* FIXME: This is lame and not portable. -aaron */
    return (unsigned long) apr_os_thread_current();
}
#endif

static int have_init_ssl = 0;

static void init_ssl_libraries(void)
{
    if (!have_init_ssl) {
#if APR_HAS_THREADS
        int i, numlocks;
#endif
        CRYPTO_malloc_init();
        ERR_load_crypto_strings();
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();

#if APR_HAS_THREADS
        numlocks = CRYPTO_num_locks();
        apr_pool_create(&ssl_pool, NULL);
        ssl_locks = apr_palloc(ssl_pool, sizeof(apr_thread_mutex_t*)*numlocks);
        for (i = 0; i < numlocks; i++) {
            apr_status_t rv;

            /* Intraprocess locks don't /need/ a filename... */
            rv = apr_thread_mutex_create(&ssl_locks[i],
                                         APR_THREAD_MUTEX_DEFAULT, ssl_pool);
            if (rv != APR_SUCCESS) {
                /* FIXME: error out here */
            }
        }
        CRYPTO_set_locking_callback(ssl_lock);
        CRYPTO_set_id_callback(ssl_id);
        CRYPTO_set_dynlock_create_callback(ssl_dyn_create);
        CRYPTO_set_dynlock_lock_callback(ssl_dyn_lock);
        CRYPTO_set_dynlock_destroy_callback(ssl_dyn_destroy);
#endif

        have_init_ssl = 1;
    }
}

static int ssl_need_client_cert(SSL *ssl, X509 **cert, EVP_PKEY **pkey)
{
    serf_ssl_context_t *ctx = SSL_get_app_data(ssl);
    apr_status_t status;

    if (ctx->cached_cert) {
        *cert = ctx->cached_cert;
        *pkey = ctx->cached_cert_pw;
        return 1;
    }

    while (ctx->cert_callback) {
        const char *cert_path;
        apr_file_t *cert_file;
        BIO *bio;
        PKCS12 *p12;
        int i;
        int retrying_success = 0;

        if (ctx->cert_file_success) {
            status = APR_SUCCESS;
            cert_path = ctx->cert_file_success;
            ctx->cert_file_success = NULL;
            retrying_success = 1;
        } else {
            status = ctx->cert_callback(ctx->cert_userdata, &cert_path);
        }

        if (status || !cert_path) {
          break;
        }

        /* Load the x.509 cert file stored in PKCS12 */
        status = apr_file_open(&cert_file, cert_path, APR_READ, APR_OS_DEFAULT,
                               ctx->pool);

        if (status) {
            continue;
        }

        bio = BIO_new(&bio_file_method);
        bio->ptr = cert_file;

        ctx->cert_path = cert_path;
        p12 = d2i_PKCS12_bio(bio, NULL);
        apr_file_close(cert_file);

        i = PKCS12_parse(p12, NULL, pkey, cert, NULL);

        if (i == 1) {
            PKCS12_free(p12);
            ctx->cached_cert = *cert;
            ctx->cached_cert_pw = *pkey;
            if (!retrying_success && ctx->cert_cache_pool) {
                const char *c;

                c = apr_pstrdup(ctx->cert_cache_pool, ctx->cert_path);

                apr_pool_userdata_setn(c, "serf:ssl:cert",
                                       apr_pool_cleanup_null,
                                       ctx->cert_cache_pool);
            }
            return 1;
        }
        else {
            int err = ERR_get_error();
            ERR_clear_error();
            if (ERR_GET_LIB(err) == ERR_LIB_PKCS12 &&
                ERR_GET_REASON(err) == PKCS12_R_MAC_VERIFY_FAILURE) {
                if (ctx->cert_pw_callback) {
                    const char *password;

                    if (ctx->cert_pw_success) {
                        status = APR_SUCCESS;
                        password = ctx->cert_pw_success;
                        ctx->cert_pw_success = NULL;
                    } else {
                        status = ctx->cert_pw_callback(ctx->cert_pw_userdata,
                                                       ctx->cert_path,
                                                       &password);
                    }

                    if (!status && password) {
                        i = PKCS12_parse(p12, password, pkey, cert, NULL);
                        if (i == 1) {
                            PKCS12_free(p12);
                            ctx->cached_cert = *cert;
                            ctx->cached_cert_pw = *pkey;
                            if (!retrying_success && ctx->cert_cache_pool) {
                                const char *c;

                                c = apr_pstrdup(ctx->cert_cache_pool,
                                                ctx->cert_path);

                                apr_pool_userdata_setn(c, "serf:ssl:cert",
                                                       apr_pool_cleanup_null,
                                                       ctx->cert_cache_pool);
                            }
                            if (!retrying_success && ctx->cert_pw_cache_pool) {
                                const char *c;

                                c = apr_pstrdup(ctx->cert_pw_cache_pool,
                                                password);

                                apr_pool_userdata_setn(c, "serf:ssl:certpw",
                                                       apr_pool_cleanup_null,
                                                       ctx->cert_pw_cache_pool);
                            }
                            return 1;
                        }
                    }
                }
                PKCS12_free(p12);
                return 0;
            }
            else {
                printf("OpenSSL cert error: %d %d %d\n", ERR_GET_LIB(err),
                       ERR_GET_FUNC(err),
                       ERR_GET_REASON(err));
                PKCS12_free(p12);
            }
        }
    }

    return 0;
}

SERF_DECLARE(void)
serf_ssl_client_cert_provider_set(serf_ssl_context_t *context,
                                  serf_ssl_need_client_cert_t callback,
                                  void *data,
                                  void *cache_pool)
{
    context->cert_callback = callback;
    context->cert_userdata = data;
    context->cert_cache_pool = cache_pool;
    if (context->cert_cache_pool) {
        apr_pool_userdata_get((void**)&context->cert_file_success,
                              "serf:ssl:cert", cache_pool);
    }
}

SERF_DECLARE(void)
serf_ssl_client_cert_password_set(serf_ssl_context_t *context,
                                  serf_ssl_need_cert_password_t callback,
                                  void *data,
                                  void *cache_pool)
{
    context->cert_pw_callback = callback;
    context->cert_pw_userdata = data;
    context->cert_pw_cache_pool = cache_pool;
    if (context->cert_pw_cache_pool) {
        apr_pool_userdata_get((void**)&context->cert_pw_success,
                              "serf:ssl:certpw", cache_pool);
    }
}

static serf_ssl_context_t *ssl_init_context(void)
{
    serf_ssl_context_t *ssl_ctx;
    apr_pool_t *pool;
    serf_bucket_alloc_t *allocator;

    init_ssl_libraries();

    apr_pool_create(&pool, NULL);
    allocator = serf_bucket_allocator_create(pool, NULL, NULL);

    ssl_ctx = serf_bucket_mem_alloc(allocator, sizeof(*ssl_ctx));

    ssl_ctx->refcount = 0;
    ssl_ctx->pool = pool;
    ssl_ctx->allocator = allocator;

    ssl_ctx->ctx = SSL_CTX_new(SSLv23_client_method());

    SSL_CTX_set_client_cert_cb(ssl_ctx->ctx, ssl_need_client_cert);
    ssl_ctx->cached_cert = 0;
    ssl_ctx->cached_cert_pw = 0;

    SSL_CTX_set_options(ssl_ctx->ctx, SSL_OP_ALL);

    ssl_ctx->ssl = SSL_new(ssl_ctx->ctx);
    ssl_ctx->bio = BIO_new(&bio_bucket_method);
    ssl_ctx->bio->ptr = ssl_ctx;

    SSL_set_bio(ssl_ctx->ssl, ssl_ctx->bio, ssl_ctx->bio);

    SSL_set_connect_state(ssl_ctx->ssl);

    SSL_set_app_data(ssl_ctx->ssl, ssl_ctx);

    ssl_ctx->encrypt.stream = NULL;
    ssl_ctx->encrypt.stream_next = NULL;
    ssl_ctx->encrypt.pending = NULL;
    ssl_ctx->encrypt.status = APR_SUCCESS;
    serf_databuf_init(&ssl_ctx->encrypt.databuf);
    ssl_ctx->encrypt.databuf.read = ssl_encrypt;
    ssl_ctx->encrypt.databuf.read_baton = ssl_ctx;

    ssl_ctx->decrypt.stream = NULL;
    ssl_ctx->decrypt.pending = NULL;
    ssl_ctx->decrypt.status = APR_SUCCESS;
    serf_databuf_init(&ssl_ctx->decrypt.databuf);
    ssl_ctx->decrypt.databuf.read = ssl_decrypt;
    ssl_ctx->decrypt.databuf.read_baton = ssl_ctx;

    return ssl_ctx;
}

static apr_status_t ssl_free_context(
    serf_ssl_context_t *ssl_ctx)
{
    apr_pool_t *p;

    /* If never had the pending buckets, don't try to free them. */
    if (ssl_ctx->decrypt.pending != NULL) {
        serf_bucket_destroy(ssl_ctx->decrypt.pending);
    }
    if (ssl_ctx->encrypt.pending != NULL) {
        serf_bucket_destroy(ssl_ctx->encrypt.pending);
    }

    /* SSL_free implicitly frees the underlying BIO. */
    SSL_free(ssl_ctx->ssl);
    SSL_CTX_free(ssl_ctx->ctx);

    p = ssl_ctx->pool;

    serf_bucket_mem_free(ssl_ctx->allocator, ssl_ctx);
    apr_pool_destroy(p);

    return APR_SUCCESS;
}

static serf_bucket_t * serf_bucket_ssl_create(
    serf_ssl_context_t *ssl_ctx,
    serf_bucket_alloc_t *allocator,
    const serf_bucket_type_t *type)
{
    ssl_context_t *ctx;

    ctx = serf_bucket_mem_alloc(allocator, sizeof(*ctx));
    if (!ssl_ctx) {
        ctx->ssl_ctx = ssl_init_context();
    }
    else {
        ctx->ssl_ctx = ssl_ctx;
    }
    ctx->ssl_ctx->refcount++;

    return serf_bucket_create(type, allocator, ctx);
}

SERF_DECLARE(serf_bucket_t *) serf_bucket_ssl_decrypt_create(
    serf_bucket_t *stream,
    serf_ssl_context_t *ssl_ctx,
    serf_bucket_alloc_t *allocator)
{
    serf_bucket_t *bkt;
    ssl_context_t *ctx;

    bkt = serf_bucket_ssl_create(ssl_ctx, allocator,
                                 &serf_bucket_type_ssl_decrypt);

    ctx = bkt->data;

    ctx->databuf = &ctx->ssl_ctx->decrypt.databuf;
    if (ctx->ssl_ctx->decrypt.stream != NULL) {
        return NULL;
    }
    ctx->ssl_ctx->decrypt.stream = stream;
    ctx->our_stream = &ctx->ssl_ctx->decrypt.stream;
    ctx->ssl_ctx->decrypt.pending =
        serf_bucket_aggregate_create(allocator);

    return bkt;
}

SERF_DECLARE(serf_ssl_context_t *) serf_bucket_ssl_decrypt_context_get(
     serf_bucket_t *bucket)
{
    ssl_context_t *ctx = bucket->data;
    return ctx->ssl_ctx;
}

SERF_DECLARE(serf_bucket_t *) serf_bucket_ssl_encrypt_create(
    serf_bucket_t *stream,
    serf_ssl_context_t *ssl_ctx,
    serf_bucket_alloc_t *allocator)
{
    serf_bucket_t *bkt;
    ssl_context_t *ctx;

    bkt = serf_bucket_ssl_create(ssl_ctx, allocator,
                                 &serf_bucket_type_ssl_encrypt);

    ctx = bkt->data;

    ctx->databuf = &ctx->ssl_ctx->encrypt.databuf;
    ctx->our_stream = &ctx->ssl_ctx->encrypt.stream;
    if (ctx->ssl_ctx->encrypt.stream == NULL) {
        ctx->ssl_ctx->encrypt.stream = stream;
        ctx->ssl_ctx->encrypt.pending =
            serf_bucket_aggregate_create(allocator);
    }
    else {
        bucket_list_t *new_list;

        new_list = serf_bucket_mem_alloc(ctx->ssl_ctx->allocator,
                                         sizeof(*new_list));
        new_list->bucket = stream;
        new_list->next = NULL;
        if (ctx->ssl_ctx->encrypt.stream_next == NULL) {
            ctx->ssl_ctx->encrypt.stream_next = new_list;
        }
        else {
            bucket_list_t *scan = ctx->ssl_ctx->encrypt.stream_next;

            while (scan->next != NULL)
                scan = scan->next;
            scan->next = new_list;
        }
    }

    return bkt;
}

SERF_DECLARE(serf_ssl_context_t *) serf_bucket_ssl_encrypt_context_get(
     serf_bucket_t *bucket)
{
    ssl_context_t *ctx = bucket->data;
    return ctx->ssl_ctx;
}

static void serf_ssl_destroy_and_data(serf_bucket_t *bucket)
{
    ssl_context_t *ctx = bucket->data;

    if (!--ctx->ssl_ctx->refcount) {
        ssl_free_context(ctx->ssl_ctx);
    }

    serf_default_destroy_and_data(bucket);
}

static void serf_ssl_decrypt_destroy_and_data(serf_bucket_t *bucket)
{
    ssl_context_t *ctx = bucket->data;

    serf_bucket_destroy(*ctx->our_stream);

    serf_ssl_destroy_and_data(bucket);
}

static void serf_ssl_encrypt_destroy_and_data(serf_bucket_t *bucket)
{
    ssl_context_t *ctx = bucket->data;
    serf_ssl_context_t *ssl_ctx = ctx->ssl_ctx;

    if (ssl_ctx->encrypt.stream == *ctx->our_stream) {
        serf_bucket_destroy(*ctx->our_stream);
        serf_bucket_destroy(ssl_ctx->encrypt.pending);

        /* Reset our encrypted status and databuf. */
        ssl_ctx->encrypt.status = APR_SUCCESS;
        ssl_ctx->encrypt.databuf.status = APR_SUCCESS;

        /* Advance to the next stream - if we have one. */
        if (ssl_ctx->encrypt.stream_next == NULL) {
            ssl_ctx->encrypt.stream = NULL;
            ssl_ctx->encrypt.pending = NULL;
        }
        else {
            bucket_list_t *cur;

            cur = ssl_ctx->encrypt.stream_next;
            ssl_ctx->encrypt.stream = cur->bucket;
            ssl_ctx->encrypt.pending =
                serf_bucket_aggregate_create(cur->bucket->allocator);
            ssl_ctx->encrypt.stream_next = cur->next;
            serf_bucket_mem_free(ssl_ctx->allocator, cur);
        }
    }
    else {
        /* Ah, darn.  We haven't sent this one along yet. */
        return;
    }
    serf_ssl_destroy_and_data(bucket);
}

static apr_status_t serf_ssl_read(serf_bucket_t *bucket,
                                  apr_size_t requested,
                                  const char **data, apr_size_t *len)
{
    ssl_context_t *ctx = bucket->data;

    return serf_databuf_read(ctx->databuf, requested, data, len);
}

static apr_status_t serf_ssl_readline(serf_bucket_t *bucket,
                                      int acceptable, int *found,
                                      const char **data,
                                      apr_size_t *len)
{
    ssl_context_t *ctx = bucket->data;

    return serf_databuf_readline(ctx->databuf, acceptable, found, data, len);
}

static apr_status_t serf_ssl_peek(serf_bucket_t *bucket,
                                  const char **data,
                                  apr_size_t *len)
{
    ssl_context_t *ctx = bucket->data;

    return serf_databuf_peek(ctx->databuf, data, len);
}


SERF_DECLARE_DATA const serf_bucket_type_t serf_bucket_type_ssl_encrypt = {
    "SSLENCRYPT",
    serf_ssl_read,
    serf_ssl_readline,
    serf_default_read_iovec,
    serf_default_read_for_sendfile,
    serf_default_read_bucket,
    serf_ssl_peek,
    serf_ssl_encrypt_destroy_and_data,
};

SERF_DECLARE_DATA const serf_bucket_type_t serf_bucket_type_ssl_decrypt = {
    "SSLDECRYPT",
    serf_ssl_read,
    serf_ssl_readline,
    serf_default_read_iovec,
    serf_default_read_for_sendfile,
    serf_default_read_bucket,
    serf_ssl_peek,
    serf_ssl_decrypt_destroy_and_data,
};
