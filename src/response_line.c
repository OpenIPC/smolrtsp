#include <smolrtsp/response_line.h>

#include "common.h"
#include "parsing.h"

#include <assert.h>
#include <string.h>

void SmolRTSP_ResponseLine_serialize(
    SmolRTSP_ResponseLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(user_writer);

    SmolRTSP_RtspVersion_serialize(self.version, user_writer, user_cx);
    user_writer(CharSlice99_from_str(" "), user_cx);
    SmolRTSP_StatusCode_serialize(self.code, user_writer, user_cx);
    user_writer(CharSlice99_from_str(" "), user_cx);
    user_writer(self.reason, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_ResponseLine_deserialize(
    SmolRTSP_ResponseLine *restrict self, CharSlice99 *restrict data,
    SmolRTSP_ResponseLineDeserializerState *restrict state) {
    assert(self);
    assert(data);
    assert(state);

    TRY_PARSE(
        SmolRTSP_ResponseLineDeserializerState_RtspVersion,
        SmolRTSP_RtspVersion_deserialize(&self->version, data));

    TRY_PARSE(
        SmolRTSP_ResponseLineDeserializerState_StatusCode,
        SmolRTSP_StatusCode_deserialize(&self->code, data));

    TRY_PARSE(
        SmolRTSP_ResponseLineDeserializerState_ReasonPhrase,
        SmolRTSP_ReasonPhrase_deserialize(&self->reason, data));

    return SmolRTSP_DeserializeResult_Ok;
}

bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs) {
    return SmolRTSP_RtspVersion_eq(lhs.version, rhs.version) && lhs.code == rhs.code &&
           CharSlice99_primitive_eq(lhs.reason, rhs.reason);
}
