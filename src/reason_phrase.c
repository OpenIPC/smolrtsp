#include <smolrtsp/reason_phrase.h>

#include <string.h>

void SmolRTSP_ReasonPhrase_serialize(
    const SmolRTSP_ReasonPhrase *restrict phrase, SmolRTSP_UserWriter user_writer, void *user_cx) {
    user_writer(strlen(phrase), (const void *)*phrase, user_cx);
}
