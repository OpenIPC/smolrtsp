![OpenIPC Logo](https://cdn.themactep.com/images/logo_openipc.png)

# SmolRTSP
[![CI](https://github.com/Hirrolot/smolrtsp/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/smolrtsp/actions)
[![Docs](https://img.shields.io/badge/docs-latest-blue)](https://openipc.org/smolrtsp/)

## Highlights

 - **Small.** SmolRTSP is a relatively small library designed to be used in resource-constrained systems (e.g., IP cameras).

 - **Unopinionated.** SmolRTSP does not prescribe contexts in which your code must be executed.

 - **Zero-copy.** SmolRTSP does not allocate dynamic memory or copy data while parsing; instead, it returns [array slices] to the user-supplied data.

 - **Battle-tested.** SmolRTSP is used extensively by [Majestic], an IP camera streamer developed by [OpenIPC].

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
