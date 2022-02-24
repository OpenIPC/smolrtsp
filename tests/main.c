#include <greatest.h>

#define SMOLRTSP_SUITE(name)                                                   \
    extern void name(void);                                                    \
    RUN_SUITE(name)

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();

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
    SMOLRTSP_SUITE(writer);

    SMOLRTSP_SUITE(transport);

    SMOLRTSP_SUITE(io_vec);

    GREATEST_MAIN_END();
}
