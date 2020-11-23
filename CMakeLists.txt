cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

include_directories(include)

add_library(
  smolrtsp
  include/smolrtsp.h
  include/smolrtsp/limits.h
  include/smolrtsp/message_body.h
  include/smolrtsp/reason_phrase.h
  include/smolrtsp/request_uri.h
  include/smolrtsp/status_code.h
  include/smolrtsp/rtsp_version.h
  include/smolrtsp/request.h
  include/smolrtsp/response.h
  include/smolrtsp/user_writer.h
  include/smolrtsp/method.h
  include/smolrtsp/header.h
  include/smolrtsp/request_parser.h
  include/smolrtsp/response_parser.h
  src/reason_phrase.c
  src/request_uri.c
  src/status_code.c
  src/rtsp_version.c
  src/method.c
  src/header.c
  src/request_parser.c
  src/response_parser.c
  src/parsing_aux.h
  src/parsing_aux.c)

foreach(TARGET ${BUILDSYSTEM_TARGETS})
  set_target_properties(TARGET PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
endforeach()
