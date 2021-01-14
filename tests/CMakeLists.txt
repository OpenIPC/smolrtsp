cmake_minimum_required(VERSION 3.0)
project(tests LANGUAGES C)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall)
endif()

include_directories(../include slice99)

add_executable(
  tests
  nala.h
  nala.c
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
  status_code.c)

add_subdirectory(.. build)
target_link_libraries(tests smolrtsp)

set_target_properties(tests PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
