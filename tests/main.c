#include "deserializers.h"

#include "nala.h"

TEST(assertions) {
    test_deserializers_header_map();
    test_deserializers_header();
    test_deserializers_message_body();
    test_deserializers_method();
    test_deserializers_reason_phrase();
    test_deserializers_request_line();
    test_deserializers_request_uri();
    test_deserializers_request();
    test_deserializers_rtsp_version();
    test_deserializers_status_code();
}
