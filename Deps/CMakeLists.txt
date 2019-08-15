CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(lkCommonDeps)

IF(WIN32)
    # force using static runtime linking
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_DEFINITIONS("/MDd")
    ELSE("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_DEFINITIONS("/MD")
    ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
ENDIF(WIN32)

# in case we get called as root CMakeLists.txt, ex from MSVC solution
IF(NOT DEFINED LKCOMMON_DEPS_DIRECTORY)
    SET(LKCOMMON_DEPS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(NOT DEFINED LKCOMMON_DEPS_DIRECTORY)

IF(DEFINED LKCOMMON_BUILD_TEST OR WIN32)
    SET(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
    ADD_SUBDIRECTORY(googletest)
ENDIF(DEFINED LKCOMMON_BUILD_TEST OR WIN32)

SET(SKIP_INSTALL_ALL ON)

ADD_SUBDIRECTORY(zlib)

SET(ZLIB_LIBRARY zlibstatic)
SET(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib/ ${CMAKE_CURRENT_BINARY_DIR}/zlib)
SET(PNG_SHARED OFF CACHE BOOL "Build shared lib")
SET(PNG_TESTS OFF CACHE BOOL "Build libpng tests")
SET(PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used")

ADD_SUBDIRECTORY(libpng)

# Workaround to keep Deps/zlib submodule clean after build
ADD_CUSTOM_TARGET(PostDepsBuild ALL DEPENDS zlibstatic png_static
    COMMENT "Post build script output:"
)

IF(WIN32)
    ADD_CUSTOM_COMMAND(TARGET PostDepsBuild POST_BUILD
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/post_build_deps.bat
    )
ELSEIF(UNIX)
    ADD_CUSTOM_COMMAND(TARGET PostDepsBuild POST_BUILD
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/post_build_deps.sh
    )
ENDIF()


### Extra variables to use later on when building lkCommon ###

# Location of pnglibconf.h
SET(LKCOMMON_DEPS_LIBPNG_CONF_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libpng
    CACHE INTERNAL "pnglibconf.h location (required artifact after libpng build)")
