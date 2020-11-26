#include <smolrtsp/reason_phrase.h>

#include <string.h>

void SmolRTSP_ReasonPhrase_serialize(
    const SmolRTSP_ReasonPhrase *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(*self), (const void *)*self, user_cx);
}
