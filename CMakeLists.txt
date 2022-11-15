cmake_minimum_required(VERSION 3.16)
project(smolrtsp LANGUAGES C)

# Fix the warnings about `DOWNLOAD_EXTRACT_TIMESTAMP` in newer CMake versions.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

option(SMOLRTSP_SHARED "Build a shared library" OFF)
option(SMOLRTSP_FULL_MACRO_EXPANSION "Show full macro expansion backtraces" OFF)

include(FetchContent)

FetchContent_Declare(
  slice99
  GIT_REPOSITORY https://github.com/Hirrolot/slice99
  GIT_TAG 7075e1e)

FetchContent_Declare(
  metalang99
  GIT_REPOSITORY https://github.com/Hirrolot/metalang99
  GIT_TAG e0eb4e1)

FetchContent_Declare(
  datatype99
  GIT_REPOSITORY https://github.com/Hirrolot/datatype99
  GIT_TAG aa24712)

FetchContent_Declare(
  interface99
  GIT_REPOSITORY https://github.com/Hirrolot/interface99
  GIT_TAG 73c7c1e)

FetchContent_MakeAvailable(slice99 datatype99 interface99)

set(SMOLRTSP_SOURCES
    include/smolrtsp/types/error.h
    include/smolrtsp/types/header_map.h
    include/smolrtsp/types/header.h
    include/smolrtsp/types/message_body.h
    include/smolrtsp/types/method.h
    include/smolrtsp/types/reason_phrase.h
    include/smolrtsp/types/request_line.h
    include/smolrtsp/types/request_uri.h
    include/smolrtsp/types/request.h
    include/smolrtsp/types/response_line.h
    include/smolrtsp/types/response.h
    include/smolrtsp/types/rtsp_version.h
    include/smolrtsp/types/status_code.h
    include/smolrtsp/types/sdp.h
    include/smolrtsp/types/rtp.h
    include/smolrtsp/nal/h264.h
    include/smolrtsp/nal/h265.h
    include/smolrtsp/nal.h
    include/smolrtsp/writer.h
    include/smolrtsp/util.h
    include/smolrtsp/transport.h
    include/smolrtsp/rtp_transport.h
    include/smolrtsp/nal_transport.h
    include/smolrtsp/droppable.h
    include/smolrtsp/controller.h
    include/smolrtsp/io_vec.h
    include/smolrtsp/context.h
    include/smolrtsp/option.h
    src/types/error.c
    src/types/header_map.c
    src/types/header.c
    src/types/message_body.c
    src/types/method.c
    src/types/parsing.c
    src/types/parsing.h
    src/types/reason_phrase.c
    src/types/request_line.c
    src/types/request_uri.c
    src/types/request.c
    src/types/response_line.c
    src/types/response.c
    src/types/rtsp_version.c
    src/types/status_code.c
    src/types/sdp.c
    src/types/rtp.c
    src/nal/h264.c
    src/nal/h265.c
    src/nal.c
    src/writer.c
    src/writer/fd.c
    src/writer/file.c
    src/writer/string.c
    src/util.c
    src/transport/tcp.c
    src/transport/udp.c
    src/rtp_transport.c
    src/nal_transport.c
    src/io_vec.c
    src/controller.c
    src/context.c
    src/macros.h
)

if(SMOLRTSP_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SMOLRTSP_SOURCES})
else()
  add_library(${PROJECT_NAME} STATIC ${SMOLRTSP_SOURCES})
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-misleading-indentation)
endif()

if(NOT SMOLRTSP_FULL_MACRO_EXPANSION)
  if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PUBLIC -fmacro-backtrace-limit=1)
  elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PUBLIC -ftrack-macro-expansion=0)
  endif()
endif()

# Precompile headers that use Datatype99/Interface99.
target_precompile_headers(
  ${PROJECT_NAME} PUBLIC
  include/smolrtsp/types/error.h
  include/smolrtsp/nal/h264.h
  include/smolrtsp/nal/h265.h
  include/smolrtsp/nal.h
  include/smolrtsp/writer.h
  include/smolrtsp/transport.h
  include/smolrtsp/droppable.h
  include/smolrtsp/controller.h
  include/smolrtsp/rtp_transport.h
  include/smolrtsp/util.h)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC slice99 metalang99 datatype99 interface99)

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
