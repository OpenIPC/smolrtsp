/**
 * @file
 * @brief An RTSP status code.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/types/error.h>
#include <smolrtsp/writer.h>

#include <stdbool.h>
#include <stdint.h>

#include <slice99.h>

/**
 * An RTSP status code.
 */
typedef uint16_t SmolRTSP_StatusCode;

/**
 * Serialises @p self into @p w.
 *
 * @param[in] self The instance to be serialised.
 * @param[in] w The writer to be provided with serialised data.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `self != NULL`
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_StatusCode_serialize(
    const SmolRTSP_StatusCode *restrict self,
    SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Parses @p data to @p self.
 */
SmolRTSP_ParseResult SmolRTSP_StatusCode_parse(
    SmolRTSP_StatusCode *restrict self,
    CharSlice99 input) SMOLRTSP_PRIV_MUST_USE;

/**
 * Tests @p lhs and @p rhs for equality.
 *
 * @pre `lhs != NULL`
 * @pre `rhs != NULL`
 */
bool SmolRTSP_StatusCode_eq(
    const SmolRTSP_StatusCode *restrict lhs,
    const SmolRTSP_StatusCode *restrict rhs) SMOLRTSP_PRIV_MUST_USE;

/**
 * `Continue`.
 */
#define SMOLRTSP_STATUS_CONTINUE 100

/**
 * `OK`.
 */
#define SMOLRTSP_STATUS_OK 200

/**
 * `Created`.
 */
#define SMOLRTSP_STATUS_CREATED 201

/**
 * `Low on Storage Space`.
 */
#define SMOLRTSP_STATUS_LOW_ON_STORAGE_SPACE 250

/**
 * `Multiple Choices`.
 */
#define SMOLRTSP_STATUS_MULTIPLE_CHOICES 300

/**
 * `Moved Permanently`.
 */
#define SMOLRTSP_STATUS_MOVED_PERMANENTLY 301

/**
 * `Moved Temporarily`.
 */
#define SMOLRTSP_STATUS_MOVED_TEMPORARILY 302

/**
 * `See Other`.
 */
#define SMOLRTSP_STATUS_SEE_OTHER 303

/**
 * `Not Modified`.
 */
#define SMOLRTSP_STATUS_NOT_MODIFIED 304

/**
 * `Use Proxy`.
 */
#define SMOLRTSP_STATUS_USE_PROXY 305

/**
 * `Bad Request`.
 */
#define SMOLRTSP_STATUS_BAD_REQUEST 400

/**
 * `Unauthorized`.
 */
#define SMOLRTSP_STATUS_UNAUTHORIZED 401

/**
 * `Payment Required`.
 */
#define SMOLRTSP_STATUS_PAYMENT_REQUIRED 402

/**
 * `Forbidden`.
 */
#define SMOLRTSP_STATUS_FORBIDDEN 403

/**
 * `Not Found`.
 */
#define SMOLRTSP_STATUS_NOT_FOUND 404

/**
 * `Method Not Allowed`.
 */
#define SMOLRTSP_STATUS_METHOD_NOT_ALLOWED 405

/**
 * `Not Acceptable`.
 */
#define SMOLRTSP_STATUS_NOT_ACCEPTABLE 406

/**
 * `Proxy Authentication Required`.
 */
#define SMOLRTSP_STATUS_PROXY_AUTHENTICATION_REQUIRED 407

/**
 * `Request Time-out`.
 */
#define SMOLRTSP_STATUS_REQUEST_TIMEOUT 408

/**
 * `Gone`.
 */
#define SMOLRTSP_STATUS_GONE 410

/**
 * `Length Required`.
 */
#define SMOLRTSP_STATUS_LENGTH_REQUIRED 411

/**
 * `Precondition Failed`.
 */
#define SMOLRTSP_STATUS_PRECONDITION_FAILED 412

/**
 * `Request Entity Too Large`.
 */
#define SMOLRTSP_STATUS_REQUEST_ENTITY_TOO_LARGE 413

/**
 * `Request-URI Too Large`.
 */
#define SMOLRTSP_STATUS_REQUEST_URI_TOO_LARGE 414

/**
 * `Unsupported Media Type`.
 */
#define SMOLRTSP_STATUS_UNSUPPORTED_MEDIA_TYPE 415

/**
 * `Parameter Not Understood`.
 */
#define SMOLRTSP_STATUS_PARAMETER_NOT_UNDERSTOOD 451

/**
 * `Conference Not Found`.
 */
#define SMOLRTSP_STATUS_CONFERENCE_NOT_FOUND 452

/**
 * `Not Enough Bandwidth`.
 */
#define SMOLRTSP_STATUS_NOT_ENOUGH_BANDWIDTH 453

/**
 * `Session Not Found`.
 */
#define SMOLRTSP_STATUS_SESSION_NOT_FOUND 454

/**
 * `Method Not Valid in This State`.
 */
#define SMOLRTSP_STATUS_METHOD_NOT_VALID_IN_THIS_STATE 455

/**
 * `Header Field Not Valid for Resource`.
 */
#define SMOLRTSP_STATUS_HEADER_FIELD_NOT_VALID_FOR_RESOURCE 456

/**
 * `Invalid Range`.
 */
#define SMOLRTSP_STATUS_INVALID_RANGE 457

/**
 * `Parameter Is Read-Only`.
 */
#define SMOLRTSP_STATUS_PARAMETER_IS_READ_ONLY 458

/**
 * `Aggregate operation not allowed`.
 */
#define SMOLRTSP_STATUS_AGGREGATE_OPERATION_NOT_ALLOWED 459

/**
 * `Only aggregate operation allowed`.
 */
#define SMOLRTSP_STATUS_ONLY_AGGREGATE_OPERATION_ALLOWED 460

/**
 * `Unsupported transport`.
 */
#define SMOLRTSP_STATUS_UNSUPPORTED_TRANSPORT 461

/**
 * `Destination unreachable`.
 */
#define SMOLRTSP_STATUS_DESTINATION_UNREACHABLE 462

/**
 * `Internal Server Error`.
 */
#define SMOLRTSP_STATUS_INTERNAL_SERVER_ERROR 500

/**
 * `Not Implemented`.
 */
#define SMOLRTSP_STATUS_NOT_IMPLEMENTED 501

/**
 * `Bad Gateway`.
 */
#define SMOLRTSP_STATUS_BAD_GATEWAY 502

/**
 * `Service Unavailable`.
 */
#define SMOLRTSP_STATUS_SERVICE_UNAVAILABLE 503

/**
 * `Gateway Time-out`.
 */
#define SMOLRTSP_STATUS_GATEWAY_TIMEOUT 504

/**
 * `RTSP Version not supported`.
 */
#define SMOLRTSP_STATUS_RTSP_VERSION_NOT_SUPPORTED 505

/**
 * `Option not supported`.
 */
#define SMOLRTSP_STATUS_OPTION_NOT_SUPPORTED 551
