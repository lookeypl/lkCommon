CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(lkCommonDeps)

IF(WIN32)
    # force using static runtime linking
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_DEFINITIONS("/MTd")
    ELSE("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_DEFINITIONS("/MT")
    ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
ENDIF(WIN32)

# in case we get called as root CMakeLists.txt, ex from MSVC solution
IF(NOT DEFINED LKCOMMON_DEPS_DIRECTORY)
    SET(LKCOMMON_DEPS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(NOT DEFINED LKCOMMON_DEPS_DIRECTORY)

IF(DEFINED LKCOMMON_BUILD_TEST)
    ADD_SUBDIRECTORY(googletest)
ENDIF(DEFINED LKCOMMON_BUILD_TEST)

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
    COMMAND ${CMAKE_COMMAND} -E rename
        ${LKCOMMON_DEPS_DIRECTORY}/zlib/zconf.h.included
        ${LKCOMMON_DEPS_DIRECTORY}/zlib/zconf.h
    COMMENT "Moving zconf.h.included back where it belongs, so Git is happy about Deps/zlib being clean"
)


### Extra variables to use later on when building lkCommon ###

# Location of pnglibconf.h
SET(LKCOMMON_DEPS_LIBPNG_CONF_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libpng
    CACHE INTERNAL "pnglibconf.h location (required artifact after libpng build)")
