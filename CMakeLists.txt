cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

include_directories(include)

add_library(
  smolrtsp
  include/smolrtsp.h
  include/method.h
  src/method.c
  include/rtsp_version.h
  include/request.h
  include/response.h
  include/writer.h
  include/header.h
  include/misc.h
  include/misc/range_unit.h
  src/misc/range_unit.c
  include/known_headers.h
  include/known_headers/accept_ranges.h
  src/known_headers/accept_ranges.c)

foreach(TARGET ${BUILDSYSTEM_TARGETS})
  set_target_properties(TARGET PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
endforeach()
