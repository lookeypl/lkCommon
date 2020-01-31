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

IF(LKCOMMON_BUILD_TEST)
    MESSAGE(STATUS "lkCommonDeps: Google Test INCLUDED")
    SET(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
    ADD_SUBDIRECTORY(googletest)
ELSE(LKCOMMON_BUILD_TEST)
    MESSAGE(STATUS "lkCommonDeps: Google Test EXCLUDED")
ENDIF(LKCOMMON_BUILD_TEST)

SET(SKIP_INSTALL_ALL ON)

ADD_SUBDIRECTORY(zlib)

SET(ZLIB_LIBRARY zlibstatic)
SET(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib/ ${CMAKE_CURRENT_BINARY_DIR}/zlib)
SET(PNG_SHARED OFF CACHE BOOL "Build shared lib")
SET(PNG_TESTS OFF CACHE BOOL "Build libpng tests")
SET(PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used")

ADD_SUBDIRECTORY(libpng)

# Custom target to perform some post-deps-build cleanups and moves
ADD_CUSTOM_TARGET(lkCommonDepsPostBuild ALL DEPENDS zlibstatic png_static
                  COMMENT "Deps post build commands"
                  )

IF(WIN32)
    ADD_CUSTOM_COMMAND(TARGET lkCommonDepsPostBuild POST_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps_post_build.bat
                       )
ELSEIF(UNIX)
    ADD_CUSTOM_COMMAND(TARGET lkCommonDepsPostBuild POST_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps_post_build.sh
                       )
ENDIF()

IF(NOT LKCOMMON_DEPS_OUTPUT_DIRECTORY)
    IF(WIN32)
        SET(LKCOMMON_DEPS_OUTPUT_DIRECTORY ${LKCOMMON_DEPS_DIRECTORY}/Bin/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE}/ CACHE INTERNAL "lkCommonDeps output dir")
    ELSEIF(UNIX)
        SET(LKCOMMON_DEPS_OUTPUT_DIRECTORY ${LKCOMMON_DEPS_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}/ CACHE INTERNAL "lkCommonDeps output dir")
    ENDIF(WIN32)
ENDIF(NOT LKCOMMON_DEPS_OUTPUT_DIRECTORY)

ADD_CUSTOM_COMMAND(TARGET lkCommonDepsPostBuild POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:zlibstatic> ${LKCOMMON_DEPS_OUTPUT_DIRECTORY}
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:png_static> ${LKCOMMON_DEPS_OUTPUT_DIRECTORY}
                   VERBATIM
                   )

IF(LKCOMMON_BUILD_TEST)
    ADD_CUSTOM_TARGET(lkCommonDepsTestsPostBuild ALL DEPENDS gtest
                      COMMENT "Deps tests post build commands"
                      )

    IF(WIN32)
        ADD_CUSTOM_COMMAND(TARGET lkCommonDepsTestsPostBuild POST_BUILD
                           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps_post_build.bat
                           )
    ELSEIF(UNIX)
        ADD_CUSTOM_COMMAND(TARGET lkCommonDepsTestsPostBuild POST_BUILD
                           COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/deps_post_build.sh
                           )
    ENDIF()

    ADD_CUSTOM_COMMAND(TARGET lkCommonDepsTestsPostBuild POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gtest> ${LKCOMMON_DEPS_OUTPUT_DIRECTORY}
                       VERBATIM
                       )
ENDIF(LKCOMMON_BUILD_TEST)

FILE(MAKE_DIRECTORY ${LKCOMMON_DEPS_OUTPUT_DIRECTORY})

### Extra variables to use later on when building lkCommon ###

# Location of pnglibconf.h
SET(LKCOMMON_DEPS_LIBPNG_CONF_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libpng
    CACHE INTERNAL "pnglibconf.h location (required artifact after libpng build)")


# Suppress unused variable warnings
SET(UNUSED_VAR_WARNING_SUPPRESSOR_IGNORE_ME_PLS
    "${LKCOMMON_BUILD_TEST}"
    )
