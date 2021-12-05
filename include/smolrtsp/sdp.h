/**
 * @file
 * <a href="https://datatracker.ietf.org/doc/html/rfc4566">RFC 4566</a>-compliant SDP stuff.
 */

#ifndef SMOLRTSP_SDP_H
#define SMOLRTSP_SDP_H

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
 * @param[in] w The function to be provided with serialised data (possibly in chunks).
 * @param[in] w_ctx Some value provided to @p w on each write.
 *
 * @pre `w != NULL`
 */
void SmolRTSP_SdpLine_serialize(SmolRTSP_SdpLine self, SmolRTSP_Writer w, void *w_ctx);

/**
 * Protocol Version (`v=`).
 */
#define SMOLRTSP_SDP_TYPE_VERSION 'v'

/**
 * Origin (`o=`).
 */
#define SMOLRTSP_SDP_TYPE_ORIGIN 'o'

/**
 * Session Name (`s=`).
 */
#define SMOLRTSP_SDP_TYPE_SESSION_NAME 's'

/**
 * Session Information (`i=`).
 */
#define SMOLRTSP_SDP_TYPE_INFO 'i'

/**
 * URI (`u=`).
 */
#define SMOLRTSP_SDP_TYPE_URI 'u'

/**
 * Email Address (`e=`).
 */
#define SMOLRTSP_SDP_TYPE_EMAIL 'e'

/**
 * Phone Number (`p=`).
 */
#define SMOLRTSP_SDP_TYPE_PHONE 'p'

/**
 * Connection Data (`c=`).
 */
#define SMOLRTSP_SDP_TYPE_CONNECTION 'c'

/**
 * Bandwidth (`b=`).
 */
#define SMOLRTSP_SDP_TYPE_BANDWIDTH 'b'

/**
 * Timing (`t=`).
 */
#define SMOLRTSP_SDP_TYPE_TIME 't'

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
#define SMOLRTSP_SDP_TYPE_ATTR 'a'

/**
 * Media Descriptions (`m=`).
 */
#define SMOLRTSP_SDP_TYPE_MEDIA 'm'

#endif // SMOLRTSP_SDP_H
