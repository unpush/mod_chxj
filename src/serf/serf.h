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

#ifndef SERF_H
#define SERF_H

/**
 * @file serf.h
 * @brief Main serf header file
 */

#include <apr.h>
#include <apr_errno.h>
#include <apr_allocator.h>
#include <apr_pools.h>
#include <apr_network_io.h>
#include <apr_time.h>

#include "serf_declare.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declare some structures */
typedef struct serf_context_t serf_context_t;

typedef struct serf_bucket_t serf_bucket_t;
typedef struct serf_bucket_type_t serf_bucket_type_t;
typedef struct serf_bucket_alloc_t serf_bucket_alloc_t;

typedef struct serf_connection_t serf_connection_t;

typedef struct serf_request_t serf_request_t;


/**
 * @defgroup serf high-level constructs
 * @ingroup serf
 * @{
 */

/**
 * Serf-specific error codes
 */
#define SERF_ERROR_RANGE 100

/* This code is for when this is the last response on this connection:
 * i.e. do not send any more requests on this connection or expect
 * any more responses.
 */
#define SERF_ERROR_CLOSING (APR_OS_START_USERERR + SERF_ERROR_RANGE + 1)
/* This code is for when the connection terminated before the request
 * could be processed on the other side.
 */
#define SERF_ERROR_REQUEST_LOST (APR_OS_START_USERERR + SERF_ERROR_RANGE + 2)

/**
 * Create a new context for serf operations.
 *
 * A serf context defines a control loop which processes multiple
 * connections simultaneously.
 *
 * The context will be allocated within @a pool.
 */
SERF_DECLARE(serf_context_t *) serf_context_create(apr_pool_t *pool);

/** @see serf_context_run should not block at all. */
#define SERF_DURATION_NOBLOCK 0
/** @see serf_context_run should run for (nearly) "forever". */
#define SERF_DURATION_FOREVER 2000000000        /* approx 1^31 */

/**
 * Run the main networking control loop.
 *
 * The set of connections defined by the serf context @a ctx are processed.
 * Any outbound data is delivered, and incoming data is made available to
 * the associated response handlers and their buckets. This function will
 * block on the network for no longer than @a duration microseconds.
 *
 * If any data is processed (incoming or outgoing), then this function will
 * return with APR_SUCCESS. Typically, the caller will just want to call it
 * again to continue processing data.
 *
 * If no activity occurs within the specified timeout duration, then
 * APR_TIMEUP is returned.
 *
 * All temporary allocations will be made in @a pool.
 */
SERF_DECLARE(apr_status_t) serf_context_run(serf_context_t *ctx,
                                            apr_short_interval_time_t duration,
                                            apr_pool_t *pool);


/** @} */

/**
 * @defgroup serf connections and requests
 * @ingroup serf
 * @{
 */

/**
 * When a connection is established, the application needs to wrap some
 * buckets around @a skt to enable serf to process incoming responses. This
 * is the control point for assembling connection-level processing logic
 * around the given socket.
 *
 * The @a setup_baton is the baton established at connection creation time.
 *
 * This callback corresponds to reading from the server. Since this is an
 * on-demand activity, we use a callback. The corresponding write operation
 * is based on the @see serf_request_deliver function, where the application
 * can assemble the appropriate bucket(s) before delivery.
 *
 * The returned bucket should live at least as long as the connection itself.
 * It is assumed that an appropriate allocator is passed in @a setup_baton.
 * ### we may want to create a connection-level allocator and pass that
 * ### along. however, that allocator would *only* be used for this
 * ### callback. it may be wasteful to create a per-conn allocator, so this
 * ### baton-based, app-responsible form might be best.
 *
 * Responsibility for the bucket is passed to the serf library. It will be
 * destroyed when the connection is closed.
 *
 * All temporary allocations should be made in @a pool.
 */
typedef serf_bucket_t * (*serf_connection_setup_t)(apr_socket_t *skt,
                                                   void *setup_baton,
                                                   apr_pool_t *pool);

/**
 * ### need to update docco w.r.t socket. became "stream" recently.
 * ### the stream does not have a barrier, this callback should generally
 * ### add a barrier around the stream before incorporating it into a
 * ### response bucket stack.
 * ### should serf add the barrier automatically to protect its data
 * ### structure? i.e. the passed bucket becomes owned rather than
 * ### borrowed. that might suit overall semantics better.
 * Accept an incoming response for @a request, and its @a socket. A bucket
 * for the response should be constructed and returned. This is the control
 * point for assembling the appropriate wrapper buckets around the socket to
 * enable processing of the incoming response.
 *
 * The @a acceptor_baton is the baton provided when the specified request
 * was created.
 *
 * The request's pool and bucket allocator should be used for any allocations
 * that need to live for the duration of the response. Care should be taken
 * to bound the amount of memory stored in this pool -- to ensure that
 * allocations are not proportional to the amount of data in the response.
 *
 * Responsibility for the bucket is passed to the serf library. It will be
 * destroyed when the response has been fully read (the bucket returns an
 * APR_EOF status from its read functions).
 *
 * All temporary allocations should be made in @a pool.
 */
/* ### do we need to return an error? */
typedef serf_bucket_t * (*serf_response_acceptor_t)(serf_request_t *request,
                                                    serf_bucket_t *stream,
                                                    void *acceptor_baton,
                                                    apr_pool_t *pool);

/**
 * Notification callback for when a connection closes.
 *
 * This callback is used to inform an application that the @a conn
 * connection has been (abnormally) closed. The @a closed_baton is the
 * baton provided when the connection was first opened. The reason for
 * closure is given in @a why, and will be APR_SUCCESS if the application
 * requested closure (by clearing the pool used to allocate this
 * connection or calling serf_connection_close).
 *
 * All temporary allocations should be made in @a pool.
 */
typedef void (*serf_connection_closed_t)(serf_connection_t *conn,
                                         void *closed_baton,
                                         apr_status_t why,
                                         apr_pool_t *pool);

/**
 * Response data has arrived and should be processed.
 *
 * Whenever response data for @a request arrives (initially, or continued data
 * arrival), this handler is invoked. The response data is available in the
 * @a response bucket. The @a handler_baton is passed along from the baton
 * provided by the request setup callback (@see serf_request_setup_t).
 *
 * The handler MUST process data from the @a response bucket until the
 * bucket's read function states it would block (see APR_STATUS_IS_EAGAIN).
 * The handler is invoked only when new data arrives. If no further data
 * arrives, and the handler does not process all available data, then the
 * system can result in a deadlock around the unprocessed, but read, data.
 *
 * The handler should return APR_EOF when the response has been fully read.
 * If calling the handler again would block, APR_EAGAIN should be returned.
 * If the handler should be invoked again, simply return APR_SUCCESS.
 *
 * Note: if the connection closed (at the request of the application, or
 * because of an (abnormal) termination) while a request is being delivered,
 * or before a response arrives, then @a response will be NULL. This is the
 * signal that the request was not delivered properly, and no further
 * response should be expected (this callback will not be invoked again).
 * If a request is injected into the connection (during this callback's
 * execution, or otherwise), then the connection will be reopened.
 *
 * All temporary allocations should be made in @a pool.
 */
typedef apr_status_t (*serf_response_handler_t)(serf_request_t *request,
                                                serf_bucket_t *response,
                                                void *handler_baton,
                                                apr_pool_t *pool);

/**
 * Create a new connection associated with the @a ctx serf context.
 *
 * A connection will be created to (eventually) connect to the address
 * specified by @a address. The address must live at least as long as
 * @a pool (thus, as long as the connection object).
 *
 * The connection object will be allocated within @a pool. Clearing or
 * destroying this pool will close the connection, and terminate any
 * outstanding requests or responses.
 *
 * When the connection is closed (upon request or because of an error),
 * then the @a closed callback is invoked, and @a closed_baton is passed.
 *
 * ### doc on setup(_baton). tweak below comment re: acceptor.
 * NULL may be passed for @a acceptor and @a closed; default implementations
 * will be used.
 *
 * Note: the connection is not made immediately. It will be opened on
 * the next call to @see serf_context_run.
 */
SERF_DECLARE(serf_connection_t *) serf_connection_create(
    serf_context_t *ctx,
    apr_sockaddr_t *address,
    serf_connection_setup_t setup,
    void *setup_baton,
    serf_connection_closed_t closed,
    void *closed_baton,
    apr_pool_t *pool);

/**
 * Reset the connection, but re-open the socket again.
 */
SERF_DECLARE(apr_status_t) serf_connection_reset(
    serf_connection_t *conn);

/**
 * Close the connection associated with @a conn and cancel all pending requests.
 *
 * The closed callback passed to serf_connection_create() will be invoked
 * with APR_SUCCESS.
 */
SERF_DECLARE(apr_status_t) serf_connection_close(
    serf_connection_t *conn);

/**
 * Setup the @a request for delivery on its connection.
 *
 * Right before this is invoked, @a pool will be built within the
 * connection's pool for the request to use.  The associated response will
 * be allocated within that subpool. An associated bucket allocator will
 * be built. These items may be fetched from the request object through
 * @see serf_request_get_pool or @see serf_request_get_alloc.
 *
 * The content of the request is specified by the @a req_bkt bucket. When
 * a response arrives, the @a acceptor callback will be invoked (along with
 * the @a acceptor_baton) to produce a response bucket. That bucket will then
 * be passed to @a handler, along with the @a handler_baton.
 *
 * The responsibility for the request bucket is passed to the request
 * object. When the request is done with the bucket, it will be destroyed.
 */
typedef apr_status_t (*serf_request_setup_t)(serf_request_t *request,
                                             void *setup_baton,
                                             serf_bucket_t **req_bkt,
                                             serf_response_acceptor_t *acceptor,
                                             void **acceptor_baton,
                                             serf_response_handler_t *handler,
                                             void **handler_baton,
                                             apr_pool_t *pool);

/**
 * Construct a request object for the @a conn connection.
 *
 * When it is time to deliver the request, the @a setup callback will
 * be invoked with the @a setup_baton passed into it to complete the
 * construction of the request object.
 *
 * If the request has not (yet) been delivered, then it may be canceled
 * with @see serf_request_cancel.
 *
 * Invoking any calls other than @see serf_request_cancel before the setup
 * callback executes is not supported.
 */
SERF_DECLARE(serf_request_t *) serf_connection_request_create(
    serf_connection_t *conn,
    serf_request_setup_t setup,
    void *setup_baton);

/**
 * Cancel the request specified by the @a request object.
 *
 * If the request has been scheduled for delivery, then its response
 * handler will be run, passing NULL for the response bucket.
 *
 * If the request has already been (partially or fully) delivered, then
 * APR_EBUSY is returned and the request is *NOT* canceled. To properly
 * cancel the request, the connection must be closed (by clearing or
 * destroying its associated pool).
 */
SERF_DECLARE(apr_status_t) serf_request_cancel(serf_request_t *request);

/**
 * Return the pool associated with @a request.
 *
 * WARNING: be very careful about the kinds of things placed into this
 * pool. In particular, all allocation should be bounded in size, rather
 * than proportional to any data stream.
 */
SERF_DECLARE(apr_pool_t *) serf_request_get_pool(
    const serf_request_t *request);

/**
 * Return the bucket allocator associated with @a request.
 */
SERF_DECLARE(serf_bucket_alloc_t *) serf_request_get_alloc(
    const serf_request_t *request);

/**
 * Return the connection associated with @a request.
 */
SERF_DECLARE(serf_connection_t *) serf_request_get_conn(
    const serf_request_t *request);

/**
 * Update the @a handler and @a handler_baton for this @a request.
 *
 * This can be called after the request has started processing -
 * subsequent data will be delivered to this new handler.
 */
SERF_DECLARE(void) serf_request_set_handler(
    serf_request_t *request,
    const serf_response_handler_t handler,
    const void **handler_baton);

/* ### maybe some connection control functions for flood? */

/** @} */


/**
 * @defgroup serf buckets
 * @ingroup serf
 * @{
 */

/** Pass as REQUESTED to the read function of a bucket to read, consume,
 * and return all available data.
 */
#define SERF_READ_ALL_AVAIL ((apr_size_t)-1)

/** Acceptable newline types for bucket->readline(). */
#define SERF_NEWLINE_CR    0x0001
#define SERF_NEWLINE_CRLF  0x0002
#define SERF_NEWLINE_LF    0x0004
#define SERF_NEWLINE_ANY   0x0007

/** Used to indicate that a newline is not present in the data buffer. */
/* ### should we make this zero? */
#define SERF_NEWLINE_NONE  0x0008

/** Used to indicate that a CR was found at the end of a buffer, and CRLF
 * was acceptable. It may be that the LF is present, but it needs to be
 * read first.
 *
 * Note: an alternative to using this symbol would be for callers to see
 * the SERF_NEWLINE_CR return value, and know that some "end of buffer" was
 * reached. While this works well for @see serf_util_readline, it does not
 * necessary work as well for buckets (there is no obvious "end of buffer",
 * although there is an "end of bucket"). The other problem with that
 * alternative is that developers might miss the condition. This symbol
 * calls out the possibility and ensures that callers will watch for it.
 */
#define SERF_NEWLINE_CRLF_SPLIT 0x0010


struct serf_bucket_type_t {

    /** name of this bucket type */
    const char *name;

    /**
     * Read (and consume) up to @a requested bytes from @a bucket.
     *
     * A pointer to the data will be returned in @a data, and its length
     * is specified by @a len.
     *
     * The data will exist until one of two conditions occur:
     *
     * 1) this bucket is destroyed
     * 2) another call to any read function or to peek()
     *
     * If an application needs the data to exist for a longer duration,
     * then it must make a copy.
     */
    apr_status_t (*read)(serf_bucket_t *bucket, apr_size_t requested,
                         const char **data, apr_size_t *len);

    /**
     * Read (and consume) a line of data from @a bucket.
     *
     * The acceptable forms of a newline are given by @a acceptable, and
     * the type found is returned in @a found. If a newline is not present
     * in the returned data, then SERF_NEWLINE_NONE is stored into @a found.
     *
     * A pointer to the data is returned in @a data, and its length is
     * specified by @a len. The data will include the newline, if present.
     *
     * Note that there is no way to limit the amount of data returned
     * by this function.
     *
     * The lifetime of the data is the same as that of the @see read
     * function above.
     */
    apr_status_t (*readline)(serf_bucket_t *bucket, int acceptable,
                             int *found,
                             const char **data, apr_size_t *len);

    /**
     * Read a set of pointer/length pairs from the bucket.
     *
     * The size of the @a vecs array is specified by @a vecs_size. The
     * bucket should fill in elements of the array, and return the number
     * used in @a vecs_used.
     *
     * Each element of @a vecs should specify a pointer to a block of
     * data and a length of that data.
     *
     * The total length of all data elements should not exceed the
     * amount specified in @a requested.
     *
     * The lifetime of the data is the same as that of the @see read
     * function above.
     */
    apr_status_t (*read_iovec)(serf_bucket_t *bucket, apr_size_t requested,
                               int vecs_size, struct iovec *vecs,
                               int *vecs_used);

    /**
     * Read data from the bucket in a form suitable for apr_socket_sendfile()
     *
     * On input, hdtr->numheaders and hdtr->numtrailers specify the size
     * of the hdtr->headers and hdtr->trailers arrays, respectively. The
     * bucket should fill in the headers and trailers, up to the specified
     * limits, and set numheaders and numtrailers to the number of iovecs
     * filled in for each item.
     *
     * @a file should be filled in with a file that can be read. If a file
     * is not available or appropriate, then NULL should be stored. The
     * file offset for the data should be stored in @a offset, and the
     * length of that data should be stored in @a len. If a file is not
     * returned, then @a offset and @a len should be ignored.
     *
     * The file position is not required to correspond to @a offset, and
     * the caller may manipulate it at will.
     *
     * The total length of all data elements, and the portion of the
     * file should not exceed the amount specified in @a requested.
     *
     * The lifetime of the data is the same as that of the @see read
     * function above.
     */
    apr_status_t (*read_for_sendfile)(serf_bucket_t *bucket,
                                      apr_size_t requested, apr_hdtr_t *hdtr,
                                      apr_file_t **file, apr_off_t *offset,
                                      apr_size_t *len);

    /**
     * Look within @a bucket for a bucket of the given @a type. The bucket
     * must be the "initial" data because it will be consumed by this
     * function. If the given bucket type is available, then read and consume
     * it, and return it to the caller.
     *
     * This function is usually used by readers that have custom handling
     * for specific bucket types (e.g. looking for a file bucket to pass
     * to apr_socket_sendfile).
     *
     * If a bucket of the given type is not found, then NULL is returned.
     *
     * The returned bucket becomes the responsibility of the caller. When
     * the caller is done with the bucket, it should be destroyed.
     */
    serf_bucket_t * (*read_bucket)(serf_bucket_t *bucket,
                                   const serf_bucket_type_t *type);

    /**
     * Peek, but don't consume, the data in @a bucket.
     *
     * Since this function is non-destructive, the implicit read size is
     * SERF_READ_ALL_AVAIL. The caller can then use whatever amount is
     * appropriate.
     *
     * The @a data parameter will point to the data, and @a len will
     * specify how much data is available. The lifetime of the data follows
     * the same rules as the @see read function above.
     *
     * Note: if the peek does not return enough data for your particular
     * use, then you must read/consume some first, then peek again.
     *
     * If the returned data represents all available data, then APR_EOF
     * will be returned. Since this function does not consume data, it
     * can return the same data repeatedly rather than blocking; thus,
     * APR_EAGAIN will never be returned.
     */
    apr_status_t (*peek)(serf_bucket_t *bucket,
                         const char **data, apr_size_t *len);

    /**
     * Destroy @a bucket, along with any associated resources.
     */
    void (*destroy)(serf_bucket_t *bucket);

    /* ### apr buckets have 'copy', 'split', and 'setaside' functions.
       ### not sure whether those will be needed in this bucket model.
    */
};

/**
 * Should the use and lifecycle of buckets be tracked?
 *
 * When tracking, the system will ensure several semantic requirements
 * of bucket use:
 *
 *   - if a bucket returns APR_EAGAIN, one of its read functions should
 *     not be called immediately. the context's run loop should be called.
 *     ### and for APR_EOF, too?
 *   - all buckets must be drained of input before returning to the
 *     context's run loop.
 *   - buckets should not be destroyed before they return APR_EOF unless
 *     the connection is closed for some reason.
 *
 * Undefine this symbol to avoid the tracking (and a performance gain).
 *
 * ### we may want to examine when/how we provide this. should it always
 * ### be compiled in? and apps select it before including this header?
 */
/* #define SERF_DEBUG_BUCKET_USE */


/* Internal macros for tracking bucket use. */
#ifdef SERF_DEBUG_BUCKET_USE
#define SERF__RECREAD(b,s) serf_debug__record_read(b,s)
#else
#define SERF__RECREAD(b,s) (s)
#endif

#define serf_bucket_read(b,r,d,l) SERF__RECREAD(b, (b)->type->read(b,r,d,l))
#define serf_bucket_readline(b,a,f,d,l) \
    SERF__RECREAD(b, (b)->type->readline(b,a,f,d,l))
#define serf_bucket_read_iovec(b,r,s,v,u) \
    SERF__RECREAD(b, (b)->type->read_iovec(b,r,s,v,u))
#define serf_bucket_read_for_sendfile(b,r,h,f,o,l) \
    SERF__RECREAD(b, (b)->type->read_for_sendfile(b,r,h,f,o,l))
#define serf_bucket_read_bucket(b,t) ((b)->type->read_bucket(b,t))
#define serf_bucket_peek(b,d,l) ((b)->type->peek(b,d,l))
#define serf_bucket_destroy(b) ((b)->type->destroy(b))

/**
 * Check whether a real error occurred. Note that bucket read functions
 * can return EOF and EAGAIN as part of their "normal" operation, so they
 * should not be considered an error.
 */
#define SERF_BUCKET_READ_ERROR(status) ((status) \
                                        && !APR_STATUS_IS_EOF(status) \
                                        && !APR_STATUS_IS_EAGAIN(status))


struct serf_bucket_t {

    /** the type of this bucket */
    const serf_bucket_type_t *type;

    /** bucket-private data */
    void *data;

    /** the allocator used for this bucket (needed at destroy time) */
    serf_bucket_alloc_t *allocator;
};


/**
 * Generic macro to construct "is TYPE" macros.
 */
#define SERF_BUCKET_CHECK(b, btype) ((b)->type == &serf_bucket_type_ ## btype)


/**
 * Notification callback for a block that was not returned to the bucket
 * allocator when its pool was destroyed.
 *
 * The block of memory is given by @a block. The baton provided when the
 * allocator was constructed is passed as @a unfreed_baton.
 */
typedef void (*serf_unfreed_func_t)(void *unfreed_baton, void *block);

/**
 * Create a new allocator for buckets.
 *
 * All buckets are associated with a serf bucket allocator. This allocator
 * will be created within @a pool and will be destroyed when that pool is
 * cleared or destroyed.
 *
 * When the allocator is destroyed, if any allocations were not explicitly
 * returned (by calling serf_bucket_mem_free), then the @a unfreed callback
 * will be invoked for each block. @a unfreed_baton will be passed to the
 * callback.
 *
 * If @a unfreed is NULL, then the library will invoke the abort() stdlib
 * call. Any failure to return memory is a bug in the application, and an
 * abort can assist with determining what kinds of memory were not freed.
 */
SERF_DECLARE(serf_bucket_alloc_t *) serf_bucket_allocator_create(
    apr_pool_t *pool,
    serf_unfreed_func_t unfreed,
    void *unfreed_baton);

/**
 * Return the pool that was used for this @a allocator.
 *
 * WARNING: the use of this pool for allocations requires a very
 *   detailed understanding of pool behaviors, the bucket system,
 *   and knowledge of the bucket's use within the overall pattern
 *   of request/response behavior.
 *
 * See design-guide.txt for more information about pool usage.
 */
SERF_DECLARE(apr_pool_t *) serf_bucket_allocator_get_pool(
    const serf_bucket_alloc_t *allocator);


/**
 * Utility structure for reading a complete line of input from a bucket.
 *
 * Since it is entirely possible for a line to be broken by APR_EAGAIN,
 * this structure can be used to accumulate the data until a complete line
 * has been read from a bucket.
 */

/* This limit applies to the line buffer functions. If an application needs
 * longer lines, then they will need to manually handle line buffering.
 */
#define SERF_LINEBUF_LIMIT 8000

typedef struct {

    /* Current state of the buffer. */
    enum {
        SERF_LINEBUF_EMPTY,
        SERF_LINEBUF_READY,
        SERF_LINEBUF_PARTIAL,
        SERF_LINEBUF_CRLF_SPLIT
    } state;

    /* How much of the buffer have we used? */
    apr_size_t used;

    /* The line is read into this buffer, minus CR/LF */
    char line[SERF_LINEBUF_LIMIT];

} serf_linebuf_t;

/**
 * Initialize the @a linebuf structure.
 */
SERF_DECLARE(void) serf_linebuf_init(serf_linebuf_t *linebuf);

/**
 * Fetch a line of text from @a bucket, accumulating the line into
 * @a linebuf. @a acceptable specifies the types of newlines which are
 * acceptable for this fetch.
 *
 * ### we should return a data/len pair so that we can avoid a copy,
 * ### rather than having callers look into our state and line buffer.
 */
SERF_DECLARE(apr_status_t) serf_linebuf_fetch(
    serf_linebuf_t *linebuf,
    serf_bucket_t *bucket,
    int acceptable);

/** @} */


/* Internal functions for bucket use and lifecycle tracking */
SERF_DECLARE(apr_status_t) serf_debug__record_read(
    const serf_bucket_t *bucket,
    apr_status_t status);
SERF_DECLARE(void) serf_debug__entered_loop(serf_bucket_alloc_t *allocator);
SERF_DECLARE(void) serf_debug__closed_conn(serf_bucket_alloc_t *allocator);
SERF_DECLARE(void) serf_debug__bucket_destroy(const serf_bucket_t *bucket);
SERF_DECLARE(void) serf_debug__bucket_alloc_check(serf_bucket_alloc_t *allocator);

/* Version info */
#define SERF_MAJOR_VERSION 0
#define SERF_MINOR_VERSION 1
#define SERF_PATCH_VERSION 2

/**
 * Check at compile time if the Serf version is at least a certain
 * level.
 * @param major The major version component of the version checked
 * for (e.g., the "1" of "1.3.0").
 * @param minor The minor version component of the version checked
 * for (e.g., the "3" of "1.3.0").
 * @param patch The patch level component of the version checked
 * for (e.g., the "0" of "1.3.0").
 */
#define SERF_VERSION_AT_LEAST(major,minor,patch)                         \
(((major) < SERF_MAJOR_VERSION)                                          \
  || ((major) == SERF_MAJOR_VERSION && (minor) < SERF_MINOR_VERSION)     \
   || ((major) == SERF_MAJOR_VERSION && (minor) == SERF_MINOR_VERSION && \
            (patch) <= SERF_PATCH_VERSION))

#ifdef __cplusplus
}
#endif


/*
 * Every user of serf will want to deal with our various bucket types.
 * Go ahead and include that header right now.
 *
 * Note: make sure this occurs outside of the C++ namespace block
 */
#include "serf_bucket_types.h"


#endif	/* !SERF_H */
