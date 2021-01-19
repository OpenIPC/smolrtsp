cmake_minimum_required(VERSION 3.0)
project(smolrtsp LANGUAGES C)

option(SMOLRTSP_SHARED "Build a shared library" OFF)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall)
endif()

file(GLOB sources include/smolrtsp.h
     ${CMAKE_CURRENT_LIST_DIR}/include/smolrtsp/*.h
     ${CMAKE_CURRENT_LIST_DIR}/src/*.c ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

include_directories(include)

if(SMOLRTSP_SHARED)
  add_library(smolrtsp SHARED ${sources})
else()
  add_library(smolrtsp STATIC ${sources})
endif()

target_link_libraries(smolrtsp m)

set_target_properties(smolrtsp PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
