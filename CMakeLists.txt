cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

include_directories(include)

add_library(
  smolrtsp
  include/smolrtsp.h
  include/rtsp_version.h
  include/limits.h
  include/message_body.h
  include/request.h
  include/response.h
  include/user_writer.h
  include/method.h
  src/method.c
  include/header.h
  src/header.c)

foreach(TARGET ${BUILDSYSTEM_TARGETS})
  set_target_properties(TARGET PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
endforeach()
