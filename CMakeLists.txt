cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

option(SMOLRTSP_STATIC "Build a static library" ON)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall)
endif()

file(GLOB sources include/smolrtsp.h
     ${CMAKE_CURRENT_LIST_DIR}/include/smolrtsp/*.h
     ${CMAKE_CURRENT_LIST_DIR}/src/*.c ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

include_directories(include)

if(SMOLRTSP_STATIC)
  add_library(smolrtsp STATIC ${sources})
else()
  add_library(smolrtsp SHARED ${sources})
endif()

target_link_libraries(smolrtsp m)

set_target_properties(smolrtsp PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
