#include <greatest.h>

#define SMOLRTSP_SUITE(name) extern void name(void)

SMOLRTSP_SUITE(header_map);
SMOLRTSP_SUITE(header);
SMOLRTSP_SUITE(message_body);
SMOLRTSP_SUITE(method);
SMOLRTSP_SUITE(reason_phrase);
SMOLRTSP_SUITE(request_line);
SMOLRTSP_SUITE(request_uri);
SMOLRTSP_SUITE(request);
SMOLRTSP_SUITE(response_line);
SMOLRTSP_SUITE(response);
SMOLRTSP_SUITE(rtsp_version);
SMOLRTSP_SUITE(sdp);
SMOLRTSP_SUITE(status_code);
SMOLRTSP_SUITE(util);

#undef SMOLRTSP_SUITE

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(header_map);
    RUN_SUITE(header);
    RUN_SUITE(message_body);
    RUN_SUITE(method);
    RUN_SUITE(reason_phrase);
    RUN_SUITE(request_line);
    RUN_SUITE(request_uri);
    RUN_SUITE(request);
    RUN_SUITE(response_line);
    RUN_SUITE(response);
    RUN_SUITE(rtsp_version);
    RUN_SUITE(sdp);
    RUN_SUITE(status_code);
    RUN_SUITE(util);

    GREATEST_MAIN_END();
}
