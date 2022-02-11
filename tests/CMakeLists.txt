cmake_minimum_required(VERSION 3.0.2)
project(tests LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Weverything -fsanitize=address -fmacro-backtrace-limit=1)
  add_link_options(-fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic -fsanitize=address -ftrack-macro-expansion=0)
  add_link_options(-fsanitize=address)
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
  header_map.c
  header.c
  message_body.c
  method.c
  reason_phrase.c
  request.c
  request_line.c
  response.c
  response_line.c
  request_uri.c
  rtsp_version.c
  status_code.c
  sdp.c
  util.c)

target_link_libraries(tests smolrtsp)
target_include_directories(tests PRIVATE ${greatest_SOURCE_DIR})

set_target_properties(tests PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
