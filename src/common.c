#include <smolrtsp/common.h>

SmolRTSP_ParseStatus SmolRTSP_ParseStatus_partial(size_t offset) {
    return (SmolRTSP_ParseStatus){.offset = offset, .is_complete = false};
}

SmolRTSP_ParseStatus SmolRTSP_ParseStatus_complete(size_t offset) {
    return (SmolRTSP_ParseStatus){.offset = offset, .is_complete = true};
}

SmolRTSP_ParseResult SmolRTSP_ParseResult_partial(size_t offset) {
    return SmolRTSP_ParseResult_Success(SmolRTSP_ParseStatus_partial(offset));
}

SmolRTSP_ParseResult SmolRTSP_ParseResult_complete(size_t offset) {
    return SmolRTSP_ParseResult_Success(SmolRTSP_ParseStatus_complete(offset));
}

bool SmolRTSP_ParseResult_is_partial(SmolRTSP_ParseResult self) {
    // Used to workaround `-Wreturn-type`.
    bool result = true;

    match(self) {
        of(SmolRTSP_ParseResult_Success, status) result = !status->is_complete;
        otherwise result = false;
    }

    return result;
}

bool SmolRTSP_ParseResult_is_complete(SmolRTSP_ParseResult self) {
    // Used to workaround `-Wreturn-type`.
    bool result = true;

    match(self) {
        of(SmolRTSP_ParseResult_Success, status) result = status->is_complete;
        otherwise result = false;
    }

    return result;
}
