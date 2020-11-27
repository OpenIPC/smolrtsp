cmake_minimum_required(VERSION 3.0)
project(tests LANGUAGES C)

include_directories(../include)

add_executable(
  tests
  main.c
  nala.h
  nala.c
  deserializers/header_map.c
  deserializers/header.c
  deserializers/message_body.c
  deserializers/method.c
  deserializers/reason_phrase.c
  deserializers/request_line.c
  deserializers/request_uri.c
  deserializers/request.c
  deserializers/rtsp_version.c
  deserializers/status_code.c)

add_subdirectory(.. build)
target_link_libraries(tests smolrtsp)

set_target_properties(tests PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
