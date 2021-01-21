#include "correctness.h"
#include "parsing.h"
#include <smolrtsp/request_line.h>

#include <string.h>

void SmolRTSP_RequestLine_serialize(
    SmolRTSP_RequestLine self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    precondition(user_writer);

    user_writer(self.method, user_cx);
    user_writer(Slice99_from_str(" "), user_cx);
    user_writer(self.uri, user_cx);
    user_writer(Slice99_from_str(" "), user_cx);
    SmolRTSP_RTSPVersion_serialize(self.version, user_writer, user_cx);
    user_writer(SMOLRTSP_CRLF, user_cx);
}

SmolRTSP_DeserializeResult SmolRTSP_RequestLine_deserialize(
    SmolRTSP_RequestLine *restrict self, Slice99 *restrict data,
    SmolRTSP_RequestLineDeserializerState *restrict state) {
    precondition(self);
    precondition(data);
    precondition(state);

    TRY_PARSE(
        SmolRTSP_RequestLineDeserializerStateMethod,
        SmolRTSP_Method_deserialize(&self->method, data));

    TRY_PARSE(
        SmolRTSP_RequestLineDeserializerStateRequestURI,
        SmolRTSP_RequestURI_deserialize(&self->uri, data));

    TRY_PARSE(
        SmolRTSP_RequestLineDeserializerStateRTSPVersion,
        SmolRTSP_RTSPVersion_deserialize(&self->version, data));

    TRY_PARSE(SmolRTSP_RequestLineDeserializerStateCRLF, SmolRTSP_match_str(data, "\r\n"));

    return SmolRTSP_DeserializeResultOk;
}

bool SmolRTSP_RequestLine_eq(SmolRTSP_RequestLine lhs, SmolRTSP_RequestLine rhs) {
    return Slice99_primitive_eq(lhs.method, rhs.method) && Slice99_primitive_eq(lhs.uri, rhs.uri) &&
           SmolRTSP_RTSPVersion_eq(lhs.version, rhs.version);
}
