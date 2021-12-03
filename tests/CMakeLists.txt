cmake_minimum_required(VERSION 3.0.2)
project(tests LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Weverything -fsanitize=address)
  add_link_options(-fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -pedantic -fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_subdirectory(.. build)

add_executable(
  tests
  nala/nala.h
  nala/nala.c
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

set_target_properties(tests PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
