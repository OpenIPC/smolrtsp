# smolrtsp
[![CI](https://github.com/Hirrolot/smolrtsp/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/smolrtsp/actions)

## Highlights

 - **Portable.** Smolrtsp is merely a set of [DTO]s accompanied with (de)serializers and other auxiliary routines. It neither depends on OS-specific functionality nor restricts contexts in which your code must be executed: you can use it everywhere you have a C99-compliant compiler and the C standard library.

 - **Extensible.** Although Smolrtsp exposes common RTSP methods, headers, and so on, it also allows custom values to be specified in the same way.

 - **Zero-copy.** Smolrtsp does not copy user-supplied data -- instead it uses [array slicing] via [Slice99].

[DTO]: https://en.wikipedia.org/wiki/Data_transfer_object
[array slicing]: https://en.wikipedia.org/wiki/Array_slicing
[Slice99]: https://github.com/Hirrolot/slice99

## Installation

 1. Clone [Smolrtsp] and [Slice99] to your project.
 2. Include the `smolrtsp/include` and `slice99` directories (`-I`). In the case of CMake:

```cmake
target_include_directories(<target> <INTERFACE|PUBLIC|PRIVATE> smolrtsp/include slice99)
```

 3. Add Smolrtsp to your build process. In case of CMake:

```cmake
add_subdirectory(smolrtsp [binary_dir])
target_link_libraries(<target> smolrtsp)
```

If you want to build a shared library, enable the compile-time CMake option `SMOLRTSP_SHARED`; otherwise, Smolrtsp will be compiled as a static library.

[Smolrtsp]: https://github.com/Hirrolot/smolrtsp
[Slice99]: https://github.com/Hirrolot/slice99
