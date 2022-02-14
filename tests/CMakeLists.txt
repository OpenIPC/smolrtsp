cmake_minimum_required(VERSION 3.0.2)
project(tests LANGUAGES C)

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
  util.c
  writer.c
  io_vec.c
  transport.c)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(tests PRIVATE -Wall -Wextra -fsanitize=address -fmacro-backtrace-limit=1)
  target_link_options(tests PRIVATE -fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -fsanitize=address
    -ftrack-macro-expansion=0 -Wno-misleading-indentation)
  target_link_options(tests PRIVATE -fsanitize=address)
endif()

target_link_libraries(tests smolrtsp)
target_include_directories(tests PRIVATE ${greatest_SOURCE_DIR})

set_target_properties(tests PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
