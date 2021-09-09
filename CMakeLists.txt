cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

option(SMOLRTSP_SHARED "Build a shared library" OFF)

set(SOURCES
    include/smolrtsp/common.h
    include/smolrtsp/header_map.h
    include/smolrtsp/header.h
    include/smolrtsp/message_body.h
    include/smolrtsp/method.h
    include/smolrtsp/reason_phrase.h
    include/smolrtsp/request_line.h
    include/smolrtsp/request_uri.h
    include/smolrtsp/request.h
    include/smolrtsp/response_line.h
    include/smolrtsp/response.h
    include/smolrtsp/rtsp_version.h
    include/smolrtsp/status_code.h
    include/smolrtsp/sdp.h
    include/smolrtsp/user_writer.h
    include/smolrtsp/util.h
    src/correctness.h
    src/header_map.c
    src/header.c
    src/message_body.c
    src/method.c
    src/parsing.c
    src/parsing.h
    src/reason_phrase.c
    src/request_line.c
    src/request_uri.c
    src/request.c
    src/response_line.c
    src/response.c
    src/rtsp_version.c
    src/status_code.c
    src/sdp.c
    src/user_writer.c
    src/util.c)

include_directories(include)

if(SMOLRTSP_SHARED)
  add_library(smolrtsp SHARED ${SOURCES})
else()
  add_library(smolrtsp STATIC ${SOURCES})
endif()

target_link_libraries(smolrtsp m)

set_target_properties(smolrtsp PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
