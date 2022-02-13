![OpenIPC Logo](https://cdn.themactep.com/images/logo_openipc.png)

# SmolRTSP
[![CI](https://github.com/Hirrolot/smolrtsp/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/smolrtsp/actions)
[![Docs](https://img.shields.io/badge/docs-latest-blue)](https://openipc.org/smolrtsp/)

## Highlights

 - **Small.** SmolRTSP is merely a set of [data transfer objects (DTO)] accompanied with (de)serializers and some auxiliary routines.

 - **Portable.** SmolRTSP does not prescribe contexts in which your code must be executed.

 - **Extensible.** Although SmolRTSP exposes common RTSP methods, headers, and so on, it also allows custom values to be specified in the same way.

 - **Zero-copy.** SmolRTSP does not copy user-supplied data -- instead it uses [array slicing] via [Slice99].

[data transfer objects (DTO)]: https://en.wikipedia.org/wiki/Data_transfer_object
[array slicing]: https://en.wikipedia.org/wiki/Array_slicing
[Slice99]: https://github.com/Hirrolot/slice99

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
