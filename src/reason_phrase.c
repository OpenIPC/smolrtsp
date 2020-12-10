#include <smolrtsp/reason_phrase.h>

#include <assert.h>
#include <string.h>

void SmolRTSP_ReasonPhrase_serialize(
    const SmolRTSP_ReasonPhrase *restrict self, SmolRTSP_UserWriter user_writer, void *user_cx) {
    assert(self);
    assert(user_writer);

    user_writer(strlen(self->data), self->data, user_cx);
}

bool SmolRTSP_ReasonPhrase_eq(const SmolRTSP_ReasonPhrase *lhs, const SmolRTSP_ReasonPhrase *rhs) {
    assert(lhs);
    assert(rhs);

    return strncmp(lhs->data, rhs->data, SMOLRTSP_REASON_PHRASE_SIZE) == 0;
}
