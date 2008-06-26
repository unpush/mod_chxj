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
 */

#ifndef SERF_BUCKET_TYPES_H
#define SERF_BUCKET_TYPES_H

#include <apr_mmap.h>

/* this header and serf.h refer to each other, so take a little extra care */
#ifndef SERF_H
#include "serf.h"
#endif

#include "serf_declare.h"


/**
 * @file serf_bucket_types.h
 * @brief serf-supported bucket types
 */
/* ### this whole file needs docco ... */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_request;
#define SERF_BUCKET_IS_REQUEST(b) SERF_BUCKET_CHECK((b), request)

SERF_DECLARE(serf_bucket_t *) serf_bucket_request_create(
    const char *method,
    const char *URI,
    serf_bucket_t *body,
    serf_bucket_alloc_t *allocator);

SERF_DECLARE(serf_bucket_t *) serf_bucket_request_get_headers(
    serf_bucket_t *request);

SERF_DECLARE(void) serf_bucket_request_become(serf_bucket_t *bucket,
                                              const char *method,
                                              const char *uri,
                                              serf_bucket_t *body);

/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_response;
#define SERF_BUCKET_IS_RESPONSE(b) SERF_BUCKET_CHECK((b), response)

SERF_DECLARE(serf_bucket_t *) serf_bucket_response_create(
    serf_bucket_t *stream,
    serf_bucket_alloc_t *allocator);

#define SERF_HTTP_VERSION(major, minor)  ((major) * 1000 + (minor))
#define SERF_HTTP_11 SERF_HTTP_VERSION(1, 1)
#define SERF_HTTP_10 SERF_HTTP_VERSION(1, 0)

typedef struct {
    int version;
    int code;
    const char *reason;
} serf_status_line;

/**
 * Return the Status-Line information, if available. This function
 * works like other bucket read functions: it may return APR_EAGAIN or
 * APR_EOF to signal the state of the bucket for reading. A return
 * value of APR_SUCCESS will always indicate that status line
 * information was returned; for other return values the caller must
 * check the version field in @a sline. A value of 0 means that the
 * data is not (yet) present.
 */
SERF_DECLARE(apr_status_t) serf_bucket_response_status(
    serf_bucket_t *bkt,
    serf_status_line *sline);

/**
 * Wait for the HTTP headers to be processed for a @a response.
 *
 * If the headers are available, APR_SUCCESS is returned.
 * If the headers aren't available, APR_EAGAIN is returned.
 */
SERF_DECLARE(apr_status_t) serf_bucket_response_wait_for_headers(
    serf_bucket_t *response);

/**
 * Get the headers bucket for @a response.
 */
SERF_DECLARE(serf_bucket_t *) serf_bucket_response_get_headers(
    serf_bucket_t *response);

/**
 * Advise the response @a bucket that this was from a HEAD request and
 * that it should not expect to see a response body.
 */
SERF_DECLARE(void) serf_bucket_response_set_head(serf_bucket_t *bucket);

/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_aggregate;
#define SERF_BUCKET_IS_AGGREGATE(b) SERF_BUCKET_CHECK((b), aggregate)

SERF_DECLARE(serf_bucket_t *) serf_bucket_aggregate_create(
    serf_bucket_alloc_t *allocator);

/** Transform @a bucket in-place into an aggregate bucket. */
SERF_DECLARE(void) serf_bucket_aggregate_become(serf_bucket_t *bucket);

SERF_DECLARE(void) serf_bucket_aggregate_prepend(
    serf_bucket_t *aggregate_bucket,
    serf_bucket_t *prepend_bucket);

SERF_DECLARE(void) serf_bucket_aggregate_append(
    serf_bucket_t *aggregate_bucket,
    serf_bucket_t *append_bucket);

SERF_DECLARE(void) serf_bucket_aggregate_prepend_iovec(
    serf_bucket_t *aggregate_bucket,
    struct iovec *vecs,
    int vecs_count);

SERF_DECLARE(void) serf_bucket_aggregate_append_iovec(
    serf_bucket_t *aggregate_bucket,
    struct iovec *vecs,
    int vecs_count);

/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_file;
#define SERF_BUCKET_IS_FILE(b) SERF_BUCKET_CHECK((b), file)

SERF_DECLARE(serf_bucket_t *) serf_bucket_file_create(
    apr_file_t *file,
    serf_bucket_alloc_t *allocator);

SERF_DECLARE(apr_file_t *) serf_bucket_file_borrow(serf_bucket_t *bkt);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_socket;
#define SERF_BUCKET_IS_SOCKET(b) SERF_BUCKET_CHECK((b), socket)

SERF_DECLARE(serf_bucket_t *) serf_bucket_socket_create(
    apr_socket_t *skt,
    serf_bucket_alloc_t *allocator);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_simple;
#define SERF_BUCKET_IS_SIMPLE(b) SERF_BUCKET_CHECK((b), simple)

typedef void (*serf_simple_freefunc_t)(void *baton, const char *data);

SERF_DECLARE(serf_bucket_t *) serf_bucket_simple_create(
    const char *data, apr_size_t len,
    serf_simple_freefunc_t freefunc,
    void *freefunc_baton,
    serf_bucket_alloc_t *allocator);

/**
 * Equivalent to serf_bucket_simple_create, except that the bucket takes
 * ownership of a private copy of the data.
 */
SERF_DECLARE(serf_bucket_t *) serf_bucket_simple_copy_create(
    const char *data, apr_size_t len,
    serf_bucket_alloc_t *allocator);

#define SERF_BUCKET_SIMPLE_STRING(s,a) \
    serf_bucket_simple_create(s, strlen(s), NULL, NULL, a);

#define SERF_BUCKET_SIMPLE_STRING_LEN(s,l,a) \
    serf_bucket_simple_create(s, l, NULL, NULL, a);

/* ==================================================================== */


/* Note: apr_mmap_t is always defined, but if APR doesn't have mmaps, then
   the caller can never create an apr_mmap_t to pass to this function. */

SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_mmap;
#define SERF_BUCKET_IS_MMAP(b) SERF_BUCKET_CHECK((b), mmap)

SERF_DECLARE(serf_bucket_t *) serf_bucket_mmap_create(
    apr_mmap_t *mmap,
    serf_bucket_alloc_t *allocator);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_headers;
#define SERF_BUCKET_IS_HEADERS(b) SERF_BUCKET_CHECK((b), headers)

SERF_DECLARE(serf_bucket_t *) serf_bucket_headers_create(
    serf_bucket_alloc_t *allocator);

/**
 * Set, default: value copied.
 *
 * Set the specified @a header within the bucket, copying the @a value
 * into space from this bucket's allocator. The header is NOT copied,
 * so it should remain in scope at least as long as the bucket.
 */
SERF_DECLARE(void) serf_bucket_headers_set(
    serf_bucket_t *headers_bucket,
    const char *header,
    const char *value);

/**
 * Set, copies: header and value copied.
 *
 * Copy the specified @a header and @a value into the bucket, using space
 * from this bucket's allocator.
 */
SERF_DECLARE(void) serf_bucket_headers_setc(
    serf_bucket_t *headers_bucket,
    const char *header,
    const char *value);

/**
 * Set, no copies.
 *
 * Set the specified @a header and @a value into the bucket, without
 * copying either attribute. Both attributes should remain in scope at
 * least as long as the bucket.
 */
SERF_DECLARE(void) serf_bucket_headers_setn(
    serf_bucket_t *headers_bucket,
    const char *header,
    const char *value);

/**
 * Set, extended: fine grained copy control of header and value.
 *
 * Set the specified @a header, with length @a header_size with the
 * @a value, and length @a value_size, into the bucket. The header will
 * be copied if @a header_copy is set, and the value is copied if
 * @a value_copy is set. If the values are not copied, then they should
 * remain in scope at least as long as the bucket.
 */
SERF_DECLARE(void) serf_bucket_headers_setx(
    serf_bucket_t *headers_bucket,
    const char *header, apr_size_t header_size, int header_copy,
    const char *value, apr_size_t value_size, int value_copy);

SERF_DECLARE(const char *) serf_bucket_headers_get(
    serf_bucket_t *headers_bucket,
    const char *header);

/**
 * @param baton opaque baton as passed to @see serf_bucket_headers_do
 * @param key The header key from this iteration through the table
 * @param value The header value from this iteration through the table
 */
typedef int (serf_bucket_headers_do_callback_fn_t)(
    void *baton,
    const char *key, 
    const char *value);

/**
 * @param headers_bucket headers to iterate over
 * @param func callback routine to invoke for every header in the bucket
 * @param baton baton to pass on each invocation to func
 */
SERF_DECLARE(void) serf_bucket_headers_do(
    serf_bucket_t *headers_bucket,
    serf_bucket_headers_do_callback_fn_t func,
    void *baton);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_chunk;
#define SERF_BUCKET_IS_CHUNK(b) SERF_BUCKET_CHECK((b), chunk)

SERF_DECLARE(serf_bucket_t *) serf_bucket_chunk_create(
    serf_bucket_t *stream,
    serf_bucket_alloc_t *allocator);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_dechunk;
#define SERF_BUCKET_IS_DECHUNK(b) SERF_BUCKET_CHECK((b), dechunk)

SERF_DECLARE(serf_bucket_t *) serf_bucket_dechunk_create(
    serf_bucket_t *stream,
    serf_bucket_alloc_t *allocator);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_deflate;
#define SERF_BUCKET_IS_DEFLATE(b) SERF_BUCKET_CHECK((b), deflate)

#define SERF_DEFLATE_GZIP 0
#define SERF_DEFLATE_DEFLATE 1

SERF_DECLARE(serf_bucket_t *) serf_bucket_deflate_create(
    serf_bucket_t *stream,
    serf_bucket_alloc_t *allocator,
    int format);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_limit;
#define SERF_BUCKET_IS_LIMIT(b) SERF_BUCKET_CHECK((b), limit)

SERF_DECLARE(serf_bucket_t *) serf_bucket_limit_create(
    serf_bucket_t *stream,
    apr_size_t limit,
    serf_bucket_alloc_t *allocator);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_ssl_encrypt;
#define SERF_BUCKET_IS_SSL_ENCRYPT(b) SERF_BUCKET_CHECK((b), ssl_encrypt)

typedef struct serf_ssl_context_t serf_ssl_context_t;

typedef apr_status_t (*serf_ssl_need_client_cert_t)(void *data,
                                                    const char **cert_path);

typedef apr_status_t (*serf_ssl_need_cert_password_t)(void *data,
                                                      const char *cert_path,
                                                      const char **password);

SERF_DECLARE(void)
serf_ssl_client_cert_provider_set(serf_ssl_context_t *context,
                                  serf_ssl_need_client_cert_t callback,
                                  void *data,
                                  void *cache_pool);

SERF_DECLARE(void)
serf_ssl_client_cert_password_set(serf_ssl_context_t *context,
                                  serf_ssl_need_cert_password_t callback,
                                  void *data,
                                  void *cache_pool);

SERF_DECLARE(serf_bucket_t *) serf_bucket_ssl_encrypt_create(
    serf_bucket_t *stream,
    serf_ssl_context_t *ssl_context,
    serf_bucket_alloc_t *allocator);

SERF_DECLARE(serf_ssl_context_t *) serf_bucket_ssl_encrypt_context_get(
    serf_bucket_t *bucket);

/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_ssl_decrypt;
#define SERF_BUCKET_IS_SSL_DECRYPT(b) SERF_BUCKET_CHECK((b), ssl_decrypt)

SERF_DECLARE(serf_bucket_t *) serf_bucket_ssl_decrypt_create(
    serf_bucket_t *stream,
    serf_ssl_context_t *ssl_context,
    serf_bucket_alloc_t *allocator);

SERF_DECLARE(serf_ssl_context_t *) serf_bucket_ssl_decrypt_context_get(
    serf_bucket_t *bucket);


/* ==================================================================== */


SERF_DECLARE_DATA extern const serf_bucket_type_t serf_bucket_type_barrier;
#define SERF_BUCKET_IS_BARRIER(b) SERF_BUCKET_CHECK((b), barrier)

SERF_DECLARE(serf_bucket_t *) serf_bucket_barrier_create(
    serf_bucket_t *stream,
    serf_bucket_alloc_t *allocator);


/* ==================================================================== */

/* ### do we need a PIPE bucket type? they are simple apr_file_t objects */


#ifdef __cplusplus
}
#endif

#endif	/* !SERF_BUCKET_TYPES_H */
