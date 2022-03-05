#include <greatest.h>

// Check that the main header compiles well.
#include <smolrtsp.h>

#define SMOLRTSP_SUITE(name)                                                   \
    extern void name(void);                                                    \
    RUN_SUITE(name)

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();

    SMOLRTSP_SUITE(types_header_map);
    SMOLRTSP_SUITE(types_header);
    SMOLRTSP_SUITE(types_message_body);
    SMOLRTSP_SUITE(types_method);
    SMOLRTSP_SUITE(types_reason_phrase);
    SMOLRTSP_SUITE(types_request_line);
    SMOLRTSP_SUITE(types_request_uri);
    SMOLRTSP_SUITE(types_request);
    SMOLRTSP_SUITE(types_response_line);
    SMOLRTSP_SUITE(types_response);
    SMOLRTSP_SUITE(types_rtsp_version);
    SMOLRTSP_SUITE(types_sdp);
    SMOLRTSP_SUITE(types_status_code);

    SMOLRTSP_SUITE(nal_h264);
    SMOLRTSP_SUITE(nal_h265);
    SMOLRTSP_SUITE(nal);

    SMOLRTSP_SUITE(util);
    SMOLRTSP_SUITE(writer);
    SMOLRTSP_SUITE(transport);
    SMOLRTSP_SUITE(io_vec);
    SMOLRTSP_SUITE(context);
    SMOLRTSP_SUITE(controller);

    GREATEST_MAIN_END();
}
