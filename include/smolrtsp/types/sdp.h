/**
 * @file
 * @brief <a href="https://datatracker.ietf.org/doc/html/rfc4566">RFC 4566</a>-compliant SDP
 * implementation.
 */

#pragma once

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
 * @pre `w.self && w.vptr`
 */
ssize_t SmolRTSP_SdpLine_serialize(SmolRTSP_SdpLine self, SmolRTSP_Writer w);

/**
 * Printfs a single SDP line to @p w.
 *
 * @param[in] ty The type of the SDP line.
 * @param[out] w The writer to be provided with SDP data.
 * @param[in] fmt The `printf`-like format string.
 *
 * @return The number of bytes written or a negative value on error.
 *
 * @pre `w.self && w.vptr`
 * @pre `fmt != NULL`
 */
ssize_t smolrtsp_sdp_printf(SmolRTSP_SdpType ty, SmolRTSP_Writer w, const char fmt[restrict], ...);

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
