# smolrtsp
[![CI](https://github.com/Hirrolot/smolrtsp/workflows/C/C++%20CI/badge.svg)](https://github.com/Hirrolot/smolrtsp/actions)

## Highlights

 - **Portable.** smolrtsp neither depends on OS-specific functionality nor restricts contexts in which your code must execute: you can use it everywhere you have the C standard library.
 - **Extensible.** Although smolrtsp exposes common RTSP methods, headers, and so on, it also allows custom values to be specified.
 - **Zero-copy.** smolrtsp does not copy user-supplied data -- instead it uses [array slicing] via [slice99].

[array slicing]: https://en.wikipedia.org/wiki/Array_slicing
[slice99]: https://github.com/Hirrolot/slice99
