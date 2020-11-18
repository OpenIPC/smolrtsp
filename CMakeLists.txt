cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

include_directories(include)

add_library(
  smolrtsp
  include/smolrtsp.h
  include/smolrtsp/limits.h
  include/smolrtsp/message_body.h
  include/smolrtsp/request.h
  include/smolrtsp/response.h
  include/smolrtsp/user_writer.h
  include/smolrtsp/method.h
  src/method.c
  include/smolrtsp/header.h
  src/header.c
  include/smolrtsp/rtsp_version.h
  src/rtsp_version.c)

foreach(TARGET ${BUILDSYSTEM_TARGETS})
  set_target_properties(TARGET PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
endforeach()
