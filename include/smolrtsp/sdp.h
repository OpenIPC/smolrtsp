/**
 * @file
 * <a href="https://datatracker.ietf.org/doc/html/rfc4566">RFC 4566</a>-compliant SDP stuff.
 */

#ifndef SMOLRTSP_SDP_H
#define SMOLRTSP_SDP_H

#include <smolrtsp/common.h>
#include <smolrtsp/user_writer.h>

#include <slice99.h>

/**
 * An SDP line type.
 */
typedef enum {
    /**
     * Protocol Version (`v=`).
     */
    SmolRTSP_SdpLineType_Version,

    /**
     * Origin (`o=`).
     */
    SmolRTSP_SdpLineType_Origin,

    /**
     * Session Name (`s=`).
     */
    SmolRTSP_SdpLineType_SessionName,

    /**
     * Connection Data (`c=`).
     */
    SmolRTSP_SdpLineType_Connection,

    /**
     * Media Descriptions (`m=`).
     */
    SmolRTSP_SdpLineType_Media,

    /**
     * Attributes (`a=`).
     */
    SmolRTSP_SdpLineType_Attr,

    /**
     * Timing (`t=`).
     */
    SmolRTSP_SdpLineType_Time,
} SmolRTSP_SdpLineType;

/**
 * An SDP line.
 */
typedef struct {
    /**
     * The type of this line.
     */
    SmolRTSP_SdpLineType ty;

    /**
     * The value of this line.
     */
    CharSlice99 value;
} SmolRTSP_SdpLine;

/**
 * Maps @p self to a constant null-terminated string.
 */
const char *SmolRTSP_SdpLineType_str(SmolRTSP_SdpLineType self);

/**
 * Serializes @p self to @p user_writer.
 *
 * @param[in] self The instance to be serialized.
 * @param[in] user_writer The function to be provided with serialized data (possibly in chunks).
 * @param[in] user_cx Some value provided to @p user_writer on each write.
 *
 * @pre `user_writer != NULL`
 */
void SmolRTSP_SdpLine_serialize(
    SmolRTSP_SdpLine self, SmolRTSP_UserWriter user_writer, void *user_cx);

#endif // SMOLRTSP_SDP_H
