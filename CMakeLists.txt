cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

include_directories(include libs/slice99)

add_library(
  smolrtsp
  include/smolrtsp.h
  include/smolrtsp/deserialization.h
  include/smolrtsp/user_writer.h
  include/smolrtsp/message_body.h
  include/smolrtsp/reason_phrase.h
  include/smolrtsp/request_uri.h
  include/smolrtsp/status_code.h
  include/smolrtsp/rtsp_version.h
  include/smolrtsp/request.h
  include/smolrtsp/request_line.h
  include/smolrtsp/response.h
  include/smolrtsp/response_line.h
  include/smolrtsp/method.h
  include/smolrtsp/header.h
  include/smolrtsp/header_map.h
  src/message_body.c
  src/reason_phrase.c
  src/request_uri.c
  src/status_code.c
  src/rtsp_version.c
  src/request.c
  src/request_line.c
  src/response.c
  src/response_line.c
  src/method.c
  src/header.c
  src/header_map.c
  src/match.h
  src/match.c)

target_link_libraries(smolrtsp m)

set_target_properties(smolrtsp PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
