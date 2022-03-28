/**
 * @file
 * @brief <a href="https://datatracker.ietf.org/doc/html/rfc4566">RFC
 * 4566</a>-compliant SDP implementation.
 */

#pragma once

#include <smolrtsp/priv/compiler_attrs.h>
#include <smolrtsp/writer.h>

#include <slice99.h>

/**
 * An SDP type (one character).
 */
typedef char SmolRTSP_SdpType;

/**
 * An SDP line.
 */
typedef struct {
    /**
     * The type of this line.
     */
    SmolRTSP_SdpType ty;

    /**
     * The value of this line.
     */
    CharSlice99 value;
} SmolRTSP_SdpLine;

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
ssize_t SmolRTSP_SdpLine_serialize(
    const SmolRTSP_SdpLine *restrict self,
    SmolRTSP_Writer w) SMOLRTSP_PRIV_MUST_USE;

/**
 * Printfs a single SDP line to @p w.
 *
 * @param[out] w The writer to be provided with SDP data.
 * @param[in] ty The type of the SDP line.
 * @param[in] fmt The `printf`-like format string.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 * @pre `fmt != NULL`
 */
ssize_t smolrtsp_sdp_printf(
    SmolRTSP_Writer w, SmolRTSP_SdpType ty, const char *restrict fmt,
    ...) SMOLRTSP_PRIV_MUST_USE SMOLRTSP_PRIV_GCC_ATTR(format(printf, 3, 4));

/**
 * Writes a sequence of SDP lines to @p w.
 *
 * @param[out] ret The variable name of type `ssize_t` that will be incremented
 * with a number of bytes written to @p w.
 * @param[out] w The variable name of type `SmolRTSP_Writer` to be provided with
 * SDP data.
 *
 * The rest of arguments represent a non-empty sequence of comma-separated
 * tuples `(ty, fmt, ...)`, where
 *  - `ty` is an SDP line type (such as #SMOLRTSP_SDP_VERSION),
 *  - `fmt` is a `printf`-like format string,
 *  - and `...` are the `fmt` parameters (may be omitted).
 *
 * Under the hood, #SMOLRTSP_SDP_DESCRIBE just generates a #smolrtsp_sdp_printf
 * function invocation for each variadic argument.
 *
 * ## Examples
 *
 * @code
 * char buffer[256] = {0};
 * const SmolRTSP_Writer sdp = smolrtsp_string_writer(buffer);
 * ssize_t ret = 0;
 *
 * SMOLRTSP_SDP_DESCRIBE(
 *     ret, sdp,
 *     (SMOLRTSP_SDP_VERSION, "0"),
 *     (SMOLRTSP_SDP_ORIGIN, "SmolRTSP 3855320066 3855320129 IN IP4 0.0.0.0"),
 *     (SMOLRTSP_SDP_SESSION_NAME, "SmolRTSP test"),
 *     (SMOLRTSP_SDP_CONNECTION, "IN IP4 %s", SERVER_IP_ADDR),
 *     (SMOLRTSP_SDP_TIME, "0 0"),
 *     (SMOLRTSP_SDP_MEDIA, "audio %d RTP/AVP %d", AUDIO_PORT,
 * AUDIO_RTP_PAYLOAD_TYPE),
 *     (SMOLRTSP_SDP_ATTR, "control:audio"));
 *
 * const char *expected =
 *     "v=0\r\n"
 *     "o=SmolRTSP 3855320066 3855320129 IN IP4 0.0.0.0\r\n"
 *     "s=SmolRTSP test\r\n"
 *     "c=IN IP4 0.0.0.0\r\n"
 *     "t=0 0\r\n"
 *     "m=audio 123 RTP/AVP 456\r\n"
 *     "a=control:audio\r\n";
 *
 * assert((ssize_t)strlen(expected) == ret);
 * assert(strcmp(expected, buffer) == 0);
 * @endcode
 */
#define SMOLRTSP_SDP_DESCRIBE(ret, w, ...)                                     \
    SMOLRTSP_PRIV_SDP_DESCRIBE(ret, w, __VA_ARGS__)

#ifndef DOXYGEN_IGNORE

#include <metalang99.h>

#define SMOLRTSP_PRIV_SDP_DESCRIBE(ret, w, ...)                                \
    do {                                                                       \
        ssize_t smolrtsp_priv_sdp_ret = 0;                                     \
        ML99_EVAL(ML99_variadicsForEach(                                       \
            ML99_compose(                                                      \
                ML99_appl(v(SMOLRTSP_PRIV_genSdpPrintf), v(ret, w)),           \
                v(ML99_untuple)),                                              \
            v(__VA_ARGS__)))                                                   \
    } while (0)

#define SMOLRTSP_PRIV_genSdpPrintf_IMPL(ret, w, ty, ...)                       \
    v({                                                                        \
        smolrtsp_priv_sdp_ret = smolrtsp_sdp_printf(w, ty, __VA_ARGS__);       \
        if (smolrtsp_priv_sdp_ret < 0) {                                       \
            break;                                                             \
        }                                                                      \
        ret += smolrtsp_priv_sdp_ret;                                          \
    })

#define SMOLRTSP_PRIV_genSdpPrintf_ARITY 2

#endif // DOXYGEN_IGNORE

/**
 * Protocol Version (`v=`).
 */
#define SMOLRTSP_SDP_VERSION 'v'

/**
 * Origin (`o=`).
 */
#define SMOLRTSP_SDP_ORIGIN 'o'

/**
 * Session Name (`s=`).
 */
#define SMOLRTSP_SDP_SESSION_NAME 's'

/**
 * Session Information (`i=`).
 */
#define SMOLRTSP_SDP_INFO 'i'

/**
 * URI (`u=`).
 */
#define SMOLRTSP_SDP_URI 'u'

/**
 * Email Address (`e=`).
 */
#define SMOLRTSP_SDP_EMAIL 'e'

/**
 * Phone Number (`p=`).
 */
#define SMOLRTSP_SDP_PHONE 'p'

/**
 * Connection Data (`c=`).
 */
#define SMOLRTSP_SDP_CONNECTION 'c'

/**
 * Bandwidth (`b=`).
 */
#define SMOLRTSP_SDP_BANDWIDTH 'b'

/**
 * Timing (`t=`).
 */
#define SMOLRTSP_SDP_TIME 't'

/**
 * Repeat Times (`r=`).
 */
#define SMOLRTSP_SDP_REPEAT 'r'

/**
 * Time Zones (`z=`).
 */
#define SMOLRTSP_SDP_TIME_ZONES 'z'

/**
 * Encryption Keys (`k=`).
 */
#define SMOLRTSP_SDP_ENCRYPTION_KEYS 'k'

/**
 * Attributes (`a=`).
 */
#define SMOLRTSP_SDP_ATTR 'a'

/**
 * Media Descriptions (`m=`).
 */
#define SMOLRTSP_SDP_MEDIA 'm'
