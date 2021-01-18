#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/response_line.h>

#include <string.h>

void SmolRTSP_ResponseLine_serialize(
    SmolRTSP_ResponseLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    SmolRTSP_RTSPVersion_serialize(self.version, user_writer, user_cx);
    SmolRTSP_StatusCode_serialize(self.code, user_writer, user_cx);
    user_writer(self.reason, user_cx);
    user_writer(Slice99_from_str("\r\n"), user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_ResponseLine_deserialize(
    SmolRTSP_ResponseLine *restrict self, Slice99 *restrict data,
    SmolRTSP_ResponseLineDeserializerState *restrict state) {
    precondition(self);
    precondition(data);
    precondition(state);

    TRY_PARSE(
        SmolRTSP_ResponseLineDeserializerStateRTSPVersion,
        SmolRTSP_RTSPVersion_deserialize(&self->version, data));

    TRY_PARSE(
        SmolRTSP_ResponseLineDeserializerStateStatusCode,
        SmolRTSP_StatusCode_deserialize(&self->code, data));

    TRY_PARSE(
        SmolRTSP_ResponseLineDeserializerStateReasonPhrase,
        SmolRTSP_ReasonPhrase_deserialize(&self->reason, data));

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_ResponseLine_eq(SmolRTSP_ResponseLine lhs, SmolRTSP_ResponseLine rhs) {
    return SmolRTSP_RTSPVersion_eq(lhs.version, rhs.version) && lhs.code == rhs.code &&
           Slice99_primitive_eq(lhs.reason, rhs.reason);
}
