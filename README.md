# smolrtsp
[![CI](https://github.com/Hirrolot/smolrtsp/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/smolrtsp/actions)

## Highlights

 - **Portable.** smolrtsp neither depends on OS-specific functionality nor restricts contexts in which your code must execute: you can use it everywhere you have the C standard library.
 - **Extensible.** Although smolrtsp exposes common RTSP methods, headers, and so on, it also allows custom values to be specified in the same way.
 - **Zero-copy.** smolrtsp does not copy user-supplied data -- instead it uses [array slicing] via [slice99].

[array slicing]: https://en.wikipedia.org/wiki/Array_slicing
[slice99]: https://github.com/Hirrolot/slice99

## Installation

 1. Clone https://github.com/Hirrolot/smolrtsp and https://github.com/Hirrolot/slice99 to your project.
 2. Include the `smolrtsp/include` and `slice99` directories (`-I`). In case of CMake:

```cmake
target_include_directories(<target> <INTERFACE|PUBLIC|PRIVATE> smolrtsp/include slice99)
```

 3. Add smolrtsp to your build process. In case of CMake:

```cmake
add_subdirectory(smolrtsp [binary_dir])
target_link_libraries(<target> smolrtsp)
```

If you want to build a shared library, enable the compile-time CMake option `SMOLRTSP_SHARED`; otherwise, smolrtsp will be compiled as a static library.
