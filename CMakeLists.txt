cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

include_directories(include libs/slice99)

add_library(
  smolrtsp
  include/smolrtsp.h
  include/smolrtsp/deserializers.h
  include/smolrtsp/user_writer.h
  include/smolrtsp/message_body.h
  include/smolrtsp/reason_phrase.h
  include/smolrtsp/request_uri.h
  include/smolrtsp/status_code.h
  include/smolrtsp/rtsp_version.h
  include/smolrtsp/request.h
  include/smolrtsp/response.h
  include/smolrtsp/method.h
  include/smolrtsp/header.h
  include/smolrtsp/header_map.h
  include/smolrtsp/request_line.h
  include/smolrtsp/crlf.h
  src/aux_macros.h
  src/status_code.c
  src/rtsp_version.c
  src/request.c
  src/header.c
  src/header_map.c
  src/request_line.c
  include/smolrtsp/deserializers/header_map.h
  include/smolrtsp/deserializers/message_body.h
  include/smolrtsp/deserializers/method.h
  include/smolrtsp/deserializers/reason_phrase.h
  include/smolrtsp/deserializers/rtsp_version.h
  include/smolrtsp/deserializers/status_code.h
  include/smolrtsp/deserializers/request_uri.h
  include/smolrtsp/deserializers/request.h
  include/smolrtsp/deserializers/request_line.h
  include/smolrtsp/deserializers/header.h
  src/deserializers/header_map.c
  src/deserializers/message_body.c
  src/deserializers/method.c
  src/deserializers/reason_phrase.c
  src/deserializers/rtsp_version.c
  src/deserializers/status_code.c
  src/deserializers/request_uri.c
  src/deserializers/request.c
  src/deserializers/request_line.c
  src/deserializers/header.c
  src/match.h
  src/match.c)

target_link_libraries(smolrtsp m)

set_target_properties(smolrtsp PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON)
