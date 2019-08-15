lkCommon
========

lkCommon is a library with commonly used code in other projects on my profile.

Most contents of this library were based on my [uni ABench project](https://github.com/lkostyra/abench).
After finishing it I decided to save parts of its code here for further development and
in case I need to use it in other projects of my own, without reimplementing everything
once again.

New functionalities are added on an "as-needed" basis. There's no development plan,
nor a feature roadmap, nor else.

Released under WTFPL license.

Modules
-------

Library's main header is `lkCommon.hpp`. It contains various useful macros and common includes for rest
of the project. Other modules were designed to be included separately (unless they depend on each other
in some way).

lkCommon library is split into various categories, each contains a set of modules available for use:
* `Math` - Various collection of math-related modules.
  * `Average` - Template for calculating an average of given type.
  * `Constants` - Various constants used throughout Math modules.
  * `Interpolator` - Base class for various interpolation routines. Supports only `Linear` so far.
  * `Matrix4` - Fast, zero-overhead implementation of 4x4 matrix, using SSE instructions.
  * `Random` - Implementation of various randomization algorithms.
  * `RingAverage` - Template for calculating an average of given type, for given collection of numbers.
  * `Utilities` - Various math-related utilities, more of one-offs which don't fit in any particular category
  * `Vector4` - Fast, zero-overhead implementation of 4-element float vector, using SSE instructions.
* `System` - Utilities related to system-specific elements.
  * `FS` - Functions useful for filesystem operations.
  * `Info` - Extractors for various info about host OS.
  * `KeyCodes` - Contains enum with unified keyboard codes. Useful in tandem with `Window` module.
  * `Library` - Allows to open a dynamic library in runtime and extract its symbols.
  * `Memory` - Memory-related functions (ex. aligned memory allocation and free).
  * `Window` - Creates an OS window and provides control over it.
  * `WindowImage` - Additional utility class, used in tandem with `Utils/Image` template class.
* `Utils` - Other various modules useful here and there
  * `ArenaAllocator` - Allocator speeding up allocating multiple small objects by using large memory arenas.
  * `ArenaObject` - Simple template overriding new/delete operators, forcing given object to be allocated using `ArenaAllocator`.
  * `ArgParser` - Argument parser class designed to easily digest argv's and make them easily reachable.
  * `Image` - Template designed to be an "image" - an MxN array of pixels. Used in tandem with `Pixel` module.
  * `ImageLoader` - Module loading images from hard drive, supporting multiple backends. Used internally by `Image`.
  * `Logger` - Logging module, providing logging macros. Supports logging to stdout, file and Visual Studio output.
  * `Pixel` - Module containing an N-component pixel, to use in tandem with `Image` class, or as an object being a multiple-component color.
  * `Sort` - Implementation of various sorting algorithms.
  * `StaticQueue` - Template with FIFO container queue implementation using fixed size and zero dynamic allocations.
  * `StaticStack` - Template with LIFO container (aka. stack) implementation using fixed size and zero dynamic allocations.
  * `StringConv` - Converters between String and WideString types. Mostly used by other modules for Windows-Linux compatibility purposes.
  * `ThreadPool` - A basic thread pool object, which assigns tasks from a queue to one of already running worker threads.
  * `Timer` - Timing module, uses high precision clocks available in the system.


Dependencies
------------

lkCommon's dependencies are:
* libpng
* zlib
* gtest (for lkCommonTest only)

All dependencies are downloadable through Git submodules:

```
$ git submodule update --init --recursive
```

Linux additionally needs libxcb to properly connect to the window system. Download them using your
favorite package manager before building the project.

On Linux all dependencies are added through CMake, so building the project will automatically add
them up and build them accordingly.

On Windows dependencies need to be built before using the library. For that, there is a Python 3.7
script called `deps_builder.py`. In order for it to work, both `cmake` and `msbuild` must be visible
in Windows' PATH environment variable.

To make building process on Windows easier, deps builder script is hooked to Visual Studio's
pre-build event and should fire up automagically, building everything. However, if you want to build
all the deps at once and not think about it anymore, just run `Deps/deps_builder.py` in your
terminal.


Building for Windows
--------------------

Fetch submodules and use provided `lkCommon.sln` solution file, compatible with Visual Studio 2017.
CMake-generated build files should work, but I never tested if Windows CMake build works properly.


Building for Linux
------------------

Fetch submodules and use cmake/make to build.

If you want to build tests for the library, add `LKCOMMON_BUILD_TEST` CMake define.
