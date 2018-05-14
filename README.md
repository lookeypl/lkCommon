lkCommon
========

lkCommon is a library with commonly used code in other projects on my profile.

Most contents of this library was based on my [uni ABench project](https://github.com/lkostyra/abench).
After finishing it I decided to save the code here for further development and
in case I need to use in other projects of my own.

Released under WTFPL license.

Modules
-------

lkCommon library contains following modules:
* `FS.hpp` - Basic functions useful for filesystem operations.
* `KeyCodes.hpp` - Contains enum with unified keyboard codes. Useful in tandem
with `Window.hpp` module.
* `Library.hpp` - Allows to open a dynamic library in runtime and extract its
symbols.
* `lkCommon.hpp` - Main library header with common preprocessor macros and
includes.
* `Logger.hpp` - Logging module, providing logging macros. Supports logging to
stdout, file and Visual Studio output.
* `Timer.hpp` - Timing module, uses high precision clocks available in the
system.
* `Window.hpp` - Creates an OS window and provides control over it.

If you want to build tests for the library, use `LKCOMMON_BUILD_TEST` CMake define.

Dependencies
------------

I try to keep lkCommon with lowest dependencies possible. Right now the library
depends only on Windows API on Windows and XCB library in Linux systems.

If you decide to build tests for the library, they require Google Test Framework
to build. The Framework can be acquired through Git submodules.


Building for Windows
--------------------

Not yet done

Building for Linux
------------------

Fetch submodules and use CMake to build.
