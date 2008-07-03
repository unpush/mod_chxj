/*
 * Copyright (C) 2005-2008 Atsushi Konno All rights reserved.
 * Copyright (C) 2005 QSDN,Inc. All rights reserved.
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
#include "chxj_serf.h"
#include "mod_chxj.h"
#include "apr_pools.h"

typedef struct __app_ctx_t     app_ctx_t;
typedef struct __handler_ctx_t handler_ctx_t;

struct __app_ctx_t {
  int                 ssl_flag;
  serf_ssl_context_t  *ssl_ctx;
  serf_bucket_alloc_t *bkt_alloc;
};

struct __handler_ctx_t {
#if APR_MAJOR_VERSION > 0
  apr_uint32_t requests_outstanding;
#else
  apr_atomic_t requests_outstanding;
#endif

  serf_response_acceptor_t acceptor;
  app_ctx_t                *acceptor_ctx;

  serf_response_handler_t  handler;

  const char *host;
  const char *method;
  const char *path;
  const char *user_agent;

  apr_status_t rv;
  const char *reason;

  char *response;
  apr_size_t response_len;
  char *post_data;
  apr_size_t post_data_len;
  apr_table_t *headers_out;
  apr_pool_t *pool;
  request_rec *r;
};

char *default_chxj_serf_get(request_rec *r, apr_pool_t *ppool, const char *url_path, int set_headers_flag, apr_size_t *response_len);
char *(*chxj_serf_get)(request_rec *r, apr_pool_t *ppool, const char *url_path, int set_headers_flag, apr_size_t *response_len) = default_chxj_serf_get;
char *default_chxj_serf_post(request_rec *r, apr_pool_t *ppool, const char *url_path, char *post_data, apr_size_t post_data_len, int set_headers_flag, apr_size_t *response_len);
char *(*chxj_serf_post)(request_rec *r, apr_pool_t *ppool, const char *url_path, char *post_data, apr_size_t post_data_len, int set_headers_flag, apr_size_t *response_len) = default_chxj_serf_post;


void
s_init(apr_pool_t *ppool, apr_pool_t **pool)
{
  apr_pool_create(pool, ppool);
  apr_atomic_init(*pool);
}



static serf_bucket_t *
s_connection_setup(apr_socket_t *skt, void *setup_ctx, apr_pool_t *UNUSED(pool))
{
  serf_bucket_t  *c;
  app_ctx_t      *ctx = (app_ctx_t *)setup_ctx;

  c = serf_bucket_socket_create(skt, ctx->bkt_alloc);
  if (ctx->ssl_flag) {
    c = serf_bucket_ssl_decrypt_create(c, ctx->ssl_ctx, ctx->bkt_alloc);
    if (!ctx->ssl_ctx) {
      ctx->ssl_ctx = serf_bucket_ssl_decrypt_context_get(c);
    }
    return c;
  }
  return c;
}


static void 
s_connection_closed(serf_connection_t *UNUSED(conn), void *UNUSED(closed_baton), apr_status_t UNUSED(why), apr_pool_t *UNUSED(pool))
{
  /* nothing */
}


static serf_bucket_t *
s_accept_response(serf_request_t *request, serf_bucket_t *stream, void *UNUSED(acceptor_baton), apr_pool_t *UNUSED(pool))
{
    serf_bucket_alloc_t *bkt_alloc;
    serf_bucket_t       *c;

    bkt_alloc = serf_request_get_alloc(request);
    c = serf_bucket_barrier_create(stream, bkt_alloc);
    return serf_bucket_response_create(c, bkt_alloc);
}


static apr_status_t 
s_handle_response(serf_request_t *UNUSED(request), serf_bucket_t *response, void *handler_ctx, apr_pool_t *pool)
{
  const char      *data;
  apr_size_t      len;
  serf_status_line sl;
  apr_status_t     rv;
  handler_ctx_t  *ctx = handler_ctx;

  rv = serf_bucket_response_status(response, &sl);
  if (rv != APR_SUCCESS) {
    if (APR_STATUS_IS_EAGAIN(rv)) {
      return rv;
    }
    ctx->rv = rv;
    apr_atomic_dec32(&ctx->requests_outstanding); 
    return rv;
  }
  ctx->reason = sl.reason;

  while (1) {
    rv = serf_bucket_read(response, 2048, &data, &len);
    if (SERF_BUCKET_READ_ERROR(rv)) {
      ctx->rv = rv;
      apr_atomic_dec32(&ctx->requests_outstanding);
      return rv;
    }

    if (! ctx->response) {
      ctx->response = apr_palloc(pool, len);
      ctx->response[0] = 0;
      ctx->response_len = 0;
    }
    else {
      char *tmp = apr_palloc(pool, ctx->response_len);
      memcpy(tmp, ctx->response, ctx->response_len);
      ctx->response = apr_palloc(pool, ctx->response_len + len);
      memcpy(ctx->response, tmp, ctx->response_len);
    }
    
    memcpy(&ctx->response[ctx->response_len], data, len);
    ctx->response_len += len;
    if (APR_STATUS_IS_EOF(rv)) {
      serf_bucket_t *hdrs;
      char *tmp_headers = "";
      hdrs = serf_bucket_response_get_headers(response);
      while (1) {
        rv = serf_bucket_read(hdrs, 2048, &data, &len);
        if (SERF_BUCKET_READ_ERROR(rv))
          return rv;
        tmp_headers = apr_pstrcat(ctx->pool, tmp_headers, apr_psprintf(ctx->pool , "%.*s", len, data), NULL);
        if (APR_STATUS_IS_EOF(rv)) {
          break;
        }
      }
      ctx->headers_out = apr_table_make(ctx->pool, 0);

      char *pstat;
      char *pair = NULL;
      for (;;) {
        pair = apr_strtok(tmp_headers, "\n", &pstat);
        if (!pair) break;
        tmp_headers = NULL;
        char *key;
        char *val;

        char *tpair = apr_pstrdup(ctx->pool, pair);
        key = tpair;
        val = strchr(tpair, ':');
        if (val) {
          *val = 0;
          val++;
          key = qs_trim_string(ctx->pool, key);
          val = qs_trim_string(ctx->pool, val);
          DBG(ctx->r, "key:[%s], val:[%s]", key, val);
          apr_table_add(ctx->headers_out, key, val);
        }
      }
      ctx->rv = APR_SUCCESS;
      apr_atomic_dec32(&ctx->requests_outstanding);
      DBG(ctx->r, "end of s_handle_response()(NORMAL)");
      return APR_EOF;
    }

    if (APR_STATUS_IS_EAGAIN(rv)) {
      DBG(ctx->r, "end of s_handle_response() (EAGAIN)");
      return rv;
    }
  }
}

static apr_status_t 
s_setup_request(serf_request_t           *request,
                void                     *setup_ctx,
                serf_bucket_t            **req_bkt,
                serf_response_acceptor_t *acceptor,
                void                     **acceptor_ctx,
                serf_response_handler_t  *handler,
                void                     **handler_ctx,
                apr_pool_t               *UNUSED(pool))
{
  handler_ctx_t *ctx = setup_ctx;
  serf_bucket_t *hdrs_bkt;
  serf_bucket_t *body_bkt = NULL;
  request_rec *r = ctx->r;
  int ii;

  if (ctx->post_data) {
    body_bkt = serf_bucket_simple_create(ctx->post_data, ctx->post_data_len, NULL, NULL, serf_request_get_alloc(request));
  }

  *req_bkt = serf_bucket_request_create(ctx->method, ctx->path, body_bkt, serf_request_get_alloc(request));
  hdrs_bkt = serf_bucket_request_get_headers(*req_bkt);


  apr_array_header_t *headers = (apr_array_header_t*)apr_table_elts(r->headers_in);
  apr_table_entry_t  *hentryp = (apr_table_entry_t*)headers->elts;
  for (ii=headers->nelts-1; ii>=0; ii--) {
    serf_bucket_headers_setc(hdrs_bkt, hentryp[ii].key, hentryp[ii].val);
    DBG(ctx->r, "key:[%s], val:[%s]", hentryp[ii].key, hentryp[ii].val);
  }
  if (ctx->post_data) {
    serf_bucket_headers_setc(hdrs_bkt, "X-Chxj-Forward", "Done");
    serf_bucket_headers_setc(hdrs_bkt, "X-Chxj-Content-Length", apr_psprintf(r->pool, "%d", ctx->post_data_len));
  }
  DBG(ctx->r, "Content-Length:[%s]", serf_bucket_headers_get(hdrs_bkt, "Content-Length"));

  apr_atomic_inc32(&(ctx->requests_outstanding));
  if (ctx->acceptor_ctx->ssl_flag) {
    serf_bucket_alloc_t *req_alloc;
    app_ctx_t *app_ctx = ctx->acceptor_ctx;

    req_alloc = serf_request_get_alloc(request);

    if (app_ctx->ssl_ctx == NULL) {
      *req_bkt = serf_bucket_ssl_encrypt_create(*req_bkt, NULL, app_ctx->bkt_alloc);
      app_ctx->ssl_ctx = serf_bucket_ssl_encrypt_context_get(*req_bkt);
    }
    else {
      *req_bkt = serf_bucket_ssl_encrypt_create(*req_bkt, app_ctx->ssl_ctx, app_ctx->bkt_alloc);
    }
  }
  *acceptor       = ctx->acceptor;
  *acceptor_ctx   = ctx->acceptor_ctx;
  *handler        = ctx->handler;
  *handler_ctx    = ctx;

  return APR_SUCCESS;
}

char *
default_chxj_serf_get(request_rec *r, apr_pool_t *ppool, const char *url_path, int set_headers_flag, apr_size_t *response_len)
{
  apr_pool_t *pool;
  apr_uri_t url;
  apr_status_t rv;
  apr_sockaddr_t *address = NULL;

  serf_context_t *context;
  serf_connection_t *connection;

  app_ctx_t app_ctx;
  handler_ctx_t handler_ctx;
  char *ret;


  s_init(ppool, &pool);

  apr_uri_parse(pool, url_path, &url);
  if (!url.port) {
    url.port = apr_uri_port_of_scheme(url.scheme);
  }
  if (!url.port) {
    url.port = 80;
  }
  if (!url.path) {
    url.path = "/";
  }
  if (!url.hostname) {
    url.hostname = "localhost";
  }

  rv = apr_sockaddr_info_get(&address, url.hostname, APR_UNSPEC, url.port, 0, pool);
  if (rv != APR_SUCCESS) {
    char buf[256];
    ERR(r, "apr_sockaddr_info_get() failed: rv:[%d|%s]", rv, apr_strerror(rv, buf, 256));
    return NULL;
  }
  memset(&app_ctx, 0, sizeof(app_ctx_t));

  app_ctx.bkt_alloc = serf_bucket_allocator_create(pool, NULL, NULL);
  if (strcasecmp(url.scheme, "https") == 0) {
    app_ctx.ssl_flag = 1;
  }

  context = serf_context_create(pool);
  connection = serf_connection_create(context, address, s_connection_setup, &app_ctx, s_connection_closed, &app_ctx, pool);

  memset(&handler_ctx, 0, sizeof(handler_ctx_t));
  handler_ctx.requests_outstanding = 0;
  handler_ctx.host = url.hostinfo;
  handler_ctx.method = "GET";
  handler_ctx.path = url.path;
  handler_ctx.user_agent = (char *)apr_table_get(r->headers_in, "User-Agent");
  handler_ctx.post_data = NULL;
  handler_ctx.post_data_len = 0;

  handler_ctx.acceptor     = s_accept_response;
  handler_ctx.acceptor_ctx = &app_ctx;
  handler_ctx.handler      = s_handle_response;
  handler_ctx.pool         = pool;
  handler_ctx.r            = r;
  handler_ctx.response_len = 0;
  handler_ctx.response     = NULL;

  serf_connection_request_create(connection, s_setup_request, &handler_ctx);

  while (1) {
    rv = serf_context_run(context, SERF_DURATION_FOREVER, pool);
    if (APR_STATUS_IS_TIMEUP(rv))
      continue;
    if (rv) {
      char buf[200];
      ERR(r, "Error running context: (%d) %s\n", rv, apr_strerror(rv, buf, sizeof(buf)));
      break;
    }
    if (!apr_atomic_read32(&handler_ctx.requests_outstanding)) {
      if (handler_ctx.rv != APR_SUCCESS) {
        char buf[200];
        ERR(r, "Error running context: (%d) %s\n", handler_ctx.rv, apr_strerror(handler_ctx.rv, buf, sizeof(buf)));
      }
      break;
    }
  }

  serf_connection_close(connection);
  ret = apr_pstrdup(ppool, handler_ctx.response);
  if (set_headers_flag) {
    r->headers_out = apr_table_copy(pool, handler_ctx.headers_out);
    *response_len = handler_ctx.response_len;
    char *contentType = (char *)apr_table_get(handler_ctx.headers_out, "Content-Type");
    if (contentType) {
      chxj_set_content_type(r, contentType);
    }
  }
  return ret;
}


char *
default_chxj_serf_post(request_rec *r, apr_pool_t *ppool, const char *url_path, char *post_data, apr_size_t post_data_len, int set_headers_flag, apr_size_t *response_len)
{
  apr_pool_t *pool;
  apr_uri_t url;
  apr_status_t rv;
  apr_sockaddr_t *address = NULL;

  serf_context_t *context;
  serf_connection_t *connection;

  app_ctx_t app_ctx;
  handler_ctx_t handler_ctx;
  char *ret;

  DBG(r, "start chxj_serf_post()");


  s_init(ppool, &pool);

  apr_uri_parse(pool, url_path, &url);
  if (!url.port) {
    url.port = apr_uri_port_of_scheme(url.scheme);
  }
  if (!url.port) {
    url.port = 80;
  }
  if (!url.path) {
    url.path = "/";
  }
  if (!url.hostname) {
    url.hostname = "localhost";
  }

  rv = apr_sockaddr_info_get(&address, url.hostname, APR_UNSPEC, url.port, 0, pool);
  if (rv != APR_SUCCESS) {
    char buf[256];
    ERR(r, "apr_sockaddr_info_get() failed: rv:[%d|%s]", rv, apr_strerror(rv, buf, 256));
    return NULL;
  }
  memset(&app_ctx, 0, sizeof(app_ctx_t));

  app_ctx.bkt_alloc = serf_bucket_allocator_create(pool, NULL, NULL);
  if (strcasecmp(url.scheme, "https") == 0) {
    app_ctx.ssl_flag = 1;
  }

  context = serf_context_create(pool);
  connection = serf_connection_create(context, address, s_connection_setup, &app_ctx, s_connection_closed, &app_ctx, pool);

  memset(&handler_ctx, 0, sizeof(handler_ctx_t));
  handler_ctx.requests_outstanding = 0;
  handler_ctx.host = url.hostinfo;
  handler_ctx.method = "POST";
  handler_ctx.path = url.path;
  handler_ctx.user_agent = (char *)apr_table_get(r->headers_in, "User-Agent");
  handler_ctx.post_data = post_data;
  handler_ctx.post_data_len = post_data_len;

  handler_ctx.acceptor     = s_accept_response;
  handler_ctx.acceptor_ctx = &app_ctx;
  handler_ctx.handler      = s_handle_response;
  handler_ctx.pool         = pool;
  handler_ctx.r            = r;
  handler_ctx.response_len = 0;
  handler_ctx.response     = NULL;

  serf_connection_request_create(connection, s_setup_request, &handler_ctx);

  while (1) {
    rv = serf_context_run(context, SERF_DURATION_FOREVER, pool);
    if (APR_STATUS_IS_TIMEUP(rv))
      continue;
    if (rv) {
      char buf[200];
      ERR(r, "Error running context: (%d) %s\n", rv, apr_strerror(rv, buf, sizeof(buf)));
      break;
    }
    if (!apr_atomic_read32(&handler_ctx.requests_outstanding)) {
      if (handler_ctx.rv != APR_SUCCESS) {
        char buf[200];
        ERR(r, "Error running context: (%d) %s\n", handler_ctx.rv, apr_strerror(handler_ctx.rv, buf, sizeof(buf)));
      }
      break;
    }
  }

  DBG(r, "end of serf request");
  DBG(r, "response:[%s][%d]", handler_ctx.response, handler_ctx.response_len);
  serf_connection_close(connection);
  ret = apr_pstrdup(ppool, handler_ctx.response);
  if (set_headers_flag) {
    r->headers_out = apr_table_copy(pool, handler_ctx.headers_out);
    *response_len = handler_ctx.response_len;
    char *contentType = (char *)apr_table_get(handler_ctx.headers_out, "Content-Type");
    if (contentType) {
      DBG(r, "response content type[%s]", contentType);
      chxj_set_content_type(r, apr_pstrdup(r->pool, contentType));
    }
  }
  DBG(r, "end chxj_serf_post()");
  return ret;
}
/*
 * vim:ts=2 et
 */
