cmake_minimum_required(VERSION 3.16)
project(tests LANGUAGES C)

# Fix the warnings about `DOWNLOAD_EXTRACT_TIMESTAMP` in newer CMake versions.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

add_subdirectory(.. build)

include(FetchContent)

FetchContent_Declare(
  greatest
  URL https://github.com/silentbicycle/greatest/archive/refs/tags/v1.5.0.tar.gz)

FetchContent_MakeAvailable(greatest)
FetchContent_GetProperties(greatest)

add_executable(
  tests
  main.c
  types/header_map.c
  types/header.c
  types/message_body.c
  types/method.c
  types/reason_phrase.c
  types/request.c
  types/request_line.c
  types/response.c
  types/response_line.c
  types/request_uri.c
  types/rtsp_version.c
  types/status_code.c
  types/sdp.c
  types/test_util.h
  nal/h264.c
  nal/h265.c
  nal.c
  util.c
  writer.c
  io_vec.c
  controller.c
  context.c
  transport.c)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(tests PRIVATE -Wall -Wextra -fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(tests PRIVATE -Wall -Wextra -fsanitize=address -Wno-misleading-indentation)
endif()

target_link_options(tests PRIVATE -fsanitize=address)

target_link_libraries(tests smolrtsp)
target_include_directories(tests PRIVATE ${greatest_SOURCE_DIR})

set_target_properties(tests PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
