cmake_minimum_required(VERSION 3.16)
project(smolrtsp LANGUAGES C)

option(SMOLRTSP_SHARED "Build a shared library" OFF)

set(SMOLRTSP_SOURCES
    include/smolrtsp/error.h
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
    include/smolrtsp/writer.h
    include/smolrtsp/util.h
    src/error.c
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
    src/writer.c
    src/util.c
)

if(SMOLRTSP_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SMOLRTSP_SOURCES})
else()
  add_library(${PROJECT_NAME} STATIC ${SMOLRTSP_SOURCES})
endif()

include(FetchContent)

FetchContent_Declare(
    slice99
    URL https://github.com/Hirrolot/slice99/archive/refs/tags/v0.7.2.tar.gz
)

FetchContent_Declare(
    datatype99
    URL https://github.com/Hirrolot/datatype99/archive/refs/tags/v1.6.1.tar.gz
)

FetchContent_MakeAvailable(slice99 datatype99)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${PROJECT_NAME} PUBLIC -fmacro-backtrace-limit=1)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${PROJECT_NAME} PUBLIC -ftrack-macro-expansion=0)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC slice99 datatype99)

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
