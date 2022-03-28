![OpenIPC Logo](https://cdn.themactep.com/images/logo_openipc.png)

# SmolRTSP
[![CI](https://github.com/Hirrolot/smolrtsp/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/smolrtsp/actions)
[![Docs](https://img.shields.io/badge/docs-latest-blue)](https://openipc.org/smolrtsp/)

## Highlights

 - **Small.** SmolRTSP is designed for use in embedded systems (e.g., IP cameras).
 - **Unopinionated.** SmolRTSP does not prescribe contexts in which your code must be executed. You can use it with bare POSIX sockets, [libevent], or any other network framework.
 - **Zero-copy.** SmolRTSP does not allocate or copy data while parsing; instead, it returns [array slices] to an original user-supplied string.
 - **Battle-tested.** SmolRTSP is used by [Majestic], an IP camera streamer developed by [OpenIPC].

[libevent]: https://libevent.org/
[array slices]: https://github.com/Hirrolot/slice99
[Majestic]: https://openipc.github.io/wiki/en/majestic-streamer.html
[OpenIPC]: https://openipc.org/

## Installation

If you use CMake, the recommended way is [`FetchContent`]:

[`FetchContent`]: https://cmake.org/cmake/help/latest/module/FetchContent.html

```cmake
include(FetchContent)

FetchContent_Declare(
    smolrtsp
    URL https://github.com/OpenIPC/smolrtsp/archive/refs/tags/v1.2.3.tar.gz # v1.2.3
)

FetchContent_MakeAvailable(smolrtsp)

target_link_libraries(MyProject smolrtsp)

# SmolRTSP relies on some macro machinery. To avoid useleless macro expansion
# errors, please write this:
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  target_compile_options(MyProject PRIVATE -fmacro-backtrace-limit=1)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_compile_options(MyProject PRIVATE -ftrack-macro-expansion=0)
endif()
```

If you want to build a shared library, enable the compile-time CMake option `SMOLRTSP_SHARED`; otherwise, SmolRTSP will be compiled as a static library.

## Usage

A simple example server that streams H.264 video and G.711 Mu-Law audio can be found at [`examples/server.c`](examples/server.c).

![server demo](media/example-server-demo.png)

Run it as follows:

```
$ cd examples/build
$ cmake .. && cmake --build .
$ sudo ./server
```

Then open a new terminal window to start playing:

```
$ ffplay rtsp://localhost
```
