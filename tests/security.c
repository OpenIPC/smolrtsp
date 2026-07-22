// Regression tests for issue #58: unbounded `alloca` on attacker-controlled
// RTSP field values caused a stack-overflow DoS. Parsing hostile input must now
// return a clean `Failure` (never crash or hang). AddressSanitizer is always on
// for the test binary, so a reintroduced overflow would also be caught here.

#include <smolrtsp/types/request.h>
#include <smolrtsp/types/response.h>

#include <greatest.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <slice99.h>

// Builds "OPTIONS * RTSP/1.0\r\n<name>: <value_len '9's>\r\nCSeq: 1\r\n\r\n"
// into a freshly malloc'd buffer of length `*out_total`. Caller frees.
static char *
make_request(const char *name, size_t value_len, size_t *out_total) {
    static const char line0[] = "OPTIONS * RTSP/1.0\r\n";
    static const char tail[] = "CSeq: 1\r\n\r\n";
    const size_t name_len = strlen(name);
    const size_t total = (sizeof line0 - 1) + name_len + 2 /* ": " */ +
                         value_len + 2 /* "\r\n" */ + (sizeof tail - 1);

    char *buf = malloc(total + 1);
    if (buf == NULL) {
        return NULL;
    }

    char *p = buf;
    memcpy(p, line0, sizeof line0 - 1), p += sizeof line0 - 1;
    memcpy(p, name, name_len), p += name_len;
    *p++ = ':', *p++ = ' ';
    memset(p, '9', value_len), p += value_len;
    *p++ = '\r', *p++ = '\n';
    memcpy(p, tail, sizeof tail - 1), p += sizeof tail - 1;
    *p = '\0';

    *out_total = total;
    return buf;
}

static bool request_fails(const char *s) {
    SmolRTSP_Request req = SmolRTSP_Request_uninit();
    return SmolRTSP_ParseResult_is_failure(
        SmolRTSP_Request_parse(&req, CharSlice99_from_str((char *)s)));
}

// A multi-megabyte header value must be rejected, not `alloca`'d onto the
// stack. Covers the mandatory `CSeq`/`Content-Length` path plus an arbitrary
// header.
TEST huge_header_value_does_not_crash(void) {
    static const size_t HUGE = 2 * 1024 * 1024; // 2 MiB
    const char *const names[] = {"CSeq", "Content-Length", "X-Padding"};

    for (size_t i = 0; i < sizeof names / sizeof *names; i++) {
        size_t total = 0;
        char *buf = make_request(names[i], HUGE, &total);
        ASSERT(buf != NULL);

        SmolRTSP_Request req = SmolRTSP_Request_uninit();
        const SmolRTSP_ParseResult ret =
            SmolRTSP_Request_parse(&req, CharSlice99_new(buf, total));
        ASSERT(SmolRTSP_ParseResult_is_failure(ret));

        free(buf);
    }

    PASS();
}

// Integer fields within the length cap must still be range-checked instead of
// silently overflowing (previously via `sscanf`).
TEST integer_fields_are_bounded(void) {
    // `CSeq` overflows uint32_t.
    ASSERT(request_fails("OPTIONS * RTSP/1.0\r\nCSeq: 4294967296\r\n\r\n"));
    // Negative `Content-Length` must be rejected, not wrapped to SIZE_MAX
    // (which previously left the parser waiting for that many body bytes).
    ASSERT(request_fails(
        "OPTIONS * RTSP/1.0\r\nCSeq: 1\r\nContent-Length: -1\r\n\r\n"));
    // RTSP version major overflows uint8_t.
    ASSERT(request_fails("OPTIONS * RTSP/999.0\r\nCSeq: 1\r\n\r\n"));

    PASS();
}

// The response status code must likewise be bounded (uint16_t).
TEST status_code_is_bounded(void) {
    SmolRTSP_Response resp = SmolRTSP_Response_uninit();
    const SmolRTSP_ParseResult ret = SmolRTSP_Response_parse(
        &resp, CharSlice99_from_str("RTSP/1.0 99999 OK\r\nCSeq: 1\r\n\r\n"));
    ASSERT(SmolRTSP_ParseResult_is_failure(ret));

    PASS();
}

// Well-formed input is unaffected by the new bounds.
TEST valid_request_still_parses(void) {
    SmolRTSP_Request req = SmolRTSP_Request_uninit();
    const SmolRTSP_ParseResult ret = SmolRTSP_Request_parse(
        &req, CharSlice99_from_str("OPTIONS * RTSP/1.0\r\nCSeq: 42\r\n\r\n"));
    ASSERT(SmolRTSP_ParseResult_is_complete(ret));
    ASSERT_EQ_FMT((uint32_t)42, req.cseq, "%u");

    PASS();
}

SUITE(security) {
    RUN_TEST(huge_header_value_does_not_crash);
    RUN_TEST(integer_fields_are_bounded);
    RUN_TEST(status_code_is_bounded);
    RUN_TEST(valid_request_still_parses);
}
