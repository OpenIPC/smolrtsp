cmake_minimum_required(VERSION 3.16)
project(examples LANGUAGES C)

add_subdirectory(.. build)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(LibEvent REQUIRED)

include(FetchContent)

FetchContent_Declare(
    smolrtsp-libevent
    GIT_REPOSITORY https://github.com/OpenIPC/smolrtsp-libevent.git
    GIT_TAG 369b1a38688122521552dee6023fcbdb9cf6aa6b
)

FetchContent_MakeAvailable(smolrtsp-libevent)

option(ENABLE_JPEGXS "Advertise a JPEG XS demo stream (RFC 9134) in addition to H.264" OFF)
option(ENABLE_AV1 "Advertise an AV1 demo stream (AOMedia AV1 RTP) in addition to H.264" OFF)

add_executable(server server.c)

target_link_libraries(server smolrtsp smolrtsp-libevent ${LIBEVENT_LIBRARIES})
target_include_directories(server PRIVATE ${LIBEVENT_INCLUDE_DIR})
set_target_properties(server PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

if(ENABLE_JPEGXS)
  target_compile_definitions(server PRIVATE ENABLE_JPEGXS)
endif()

if(ENABLE_AV1)
  target_compile_definitions(server PRIVATE ENABLE_AV1)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(server PRIVATE -Wall -Wextra -fsanitize=address)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(server PRIVATE -Wall -Wextra -fsanitize=address -Wno-misleading-indentation)
endif()

target_link_options(server PRIVATE -fsanitize=address)

execute_process(COMMAND xxd -i ../media/audio.g711a ../media/audio.g711a.h)
execute_process(COMMAND xxd -i ../media/video.h264 ../media/video.h264.h)

if(ENABLE_JPEGXS)
  execute_process(COMMAND xxd -i ../media/video.jxs ../media/video.jxs.h)
endif()

if(ENABLE_AV1)
  execute_process(COMMAND xxd -i ../media/video.av1 ../media/video.av1.h)
endif()
