SET(LKCOMMON_SRCS source/Allocators/ArenaAllocator.cpp
                  source/Math/CubicInterpolator.cpp
                  source/Math/Interpolator.cpp
                  source/Math/LinearInterpolator.cpp
                  source/Math/Matrix4.cpp
                  source/Math/Utilities.cpp
                  source/Math/Vector2.cpp
                  source/Math/Vector3.cpp
                  source/Math/Vector4.cpp
                  source/Utils/ArgParser.cpp
                  source/Utils/ImageLoader.cpp
                  source/Utils/ThreadPool.cpp
                  source/Internal/ImageLoaders/PNGImageLoader.cpp
                  )

SET(LKCOMMON_HDRS include/lkCommon/lkCommon.hpp
                  include/lkCommon/Allocators/ArenaAllocator.hpp
                  include/lkCommon/Allocators/ArenaObject.hpp
                  include/lkCommon/Math/Average.hpp
                  include/lkCommon/Math/AverageImpl.hpp
                  include/lkCommon/Math/Constants.hpp
                  include/lkCommon/Math/CubicInterpolator.hpp
                  include/lkCommon/Math/Interpolator.hpp
                  include/lkCommon/Math/LinearInterpolator.hpp
                  include/lkCommon/Math/Matrix4.hpp
                  include/lkCommon/Math/Matrix4Impl.hpp
                  include/lkCommon/Math/Random.hpp
                  include/lkCommon/Math/RandomImpl.hpp
                  include/lkCommon/Math/RingAverage.hpp
                  include/lkCommon/Math/RingAverageImpl.hpp
                  include/lkCommon/Math/Utilities.hpp
                  include/lkCommon/Math/UtilitiesImpl.hpp
                  include/lkCommon/Math/Vector2.hpp
                  include/lkCommon/Math/Vector2Impl.hpp
                  include/lkCommon/Math/Vector3.hpp
                  include/lkCommon/Math/Vector3Impl.hpp
                  include/lkCommon/Math/Vector4.hpp
                  include/lkCommon/Math/Vector4Impl.hpp
                  include/lkCommon/System/FS.hpp
                  include/lkCommon/System/Info.hpp
                  include/lkCommon/System/KeyCodes.hpp
                  include/lkCommon/System/Library.hpp
                  include/lkCommon/System/Memory.hpp
                  include/lkCommon/System/Window.hpp
                  include/lkCommon/System/WindowImage.hpp
                  include/lkCommon/Utils/ArgParser.hpp
                  include/lkCommon/Utils/Image.hpp
                  include/lkCommon/Utils/ImageImpl.hpp
                  include/lkCommon/Utils/ImageLoader.hpp
                  include/lkCommon/Utils/Logger.hpp
                  include/lkCommon/Utils/Pixel.hpp
                  include/lkCommon/Utils/PixelImpl.hpp
                  include/lkCommon/Utils/Sort.hpp
                  include/lkCommon/Utils/SortImpl.hpp
                  include/lkCommon/Utils/StaticQueue.hpp
                  include/lkCommon/Utils/StaticQueueImpl.hpp
                  include/lkCommon/Utils/StaticStack.hpp
                  include/lkCommon/Utils/StaticStackImpl.hpp
                  include/lkCommon/Utils/StringConv.hpp
                  include/lkCommon/Utils/ThreadPool.hpp
                  include/lkCommon/Utils/Timer.hpp
                  source/Internal/ImageLoaders/PNGImageLoader.hpp
                  )



IF(WIN32)
    SET(LKCOMMON_PLATFORM_SRCS source/System/Win/FS.cpp
                               source/System/Win/Info.cpp
                               source/System/Win/Library.cpp
                               source/System/Win/Memory.cpp
                               source/System/Win/Window.cpp
                               source/System/Win/WindowImage.cpp
                               source/Utils/Win/Logger.cpp
                               source/Utils/Win/StringConv.cpp
                               source/Utils/Win/Timer.cpp
                               source/Utils/Win/UUID.cpp
                               )
ELSEIF(UNIX)
    SET(LKCOMMON_PLATFORM_SRCS source/System/Linux/FS.cpp
                               source/System/Linux/Info.cpp
                               source/System/Linux/Library.cpp
                               source/System/Linux/Memory.cpp
                               source/System/Linux/Window.cpp
                               source/System/Linux/WindowImage.cpp
                               source/Utils/Linux/Logger.cpp
                               source/Utils/Linux/StringConv.cpp
                               source/Utils/Linux/Timer.cpp
                               source/Utils/Linux/UUID.cpp
                               source/Internal/Linux/XConnection.cpp
                               )
ELSE()
    MESSAGE(FATAL_ERROR "Platform unsupported")
ENDIF()

ADD_LIBRARY(${LKCOMMON_TARGET}
            ${LKCOMMON_SRCS} ${LKCOMMON_HDRS}
            ${LKCOMMON_PLATFORM_SRCS}
            )

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TARGET}
                           PRIVATE ${LKCOMMON_DIRECTORY}/include/
                           PRIVATE ${LKCOMMON_DIRECTORY}/source/Internal
                           PRIVATE ${LKCOMMON_DEPS_DIRECTORY}/zlib
                           PRIVATE ${LKCOMMON_DEPS_DIRECTORY}/libpng
                           PRIVATE ${LKCOMMON_DEPS_LIBPNG_CONF_INCLUDE_DIR}
                           )

# Compile options depend on platform
IF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                           PRIVATE -std:c++14
                           PRIVATE -arch:SSE2
                           )

    # Inform MSVC about preprocessor defines
    TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TARGET}
                               PRIVATE _UNICODE
                               PRIVATE _CRT_SECURE_NO_WARNINGS
                               )

    IF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                               PRIVATE -Od
                               PRIVATE -Zi
                               PRIVATE -DEBUG:FULL
                               )
        TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TARGET}
                                   PRIVATE _DEBUG
                                   )
    ELSE(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                               PRIVATE -O2
                               PRIVATE -Oi
                               )
    ENDIF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
ELSE(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                           PRIVATE "-std=c++1y"
                           PRIVATE "-msse4.1"
                           PRIVATE "-fPIC"
                           )

    IF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                               PRIVATE "-O0"
                               PRIVATE "-ggdb"
                               )
        TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TARGET}
                                   PRIVATE _DEBUG
                                   )
    ELSE(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                               PRIVATE "-O2"
                               )
    ENDIF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
ENDIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

IF(LKCOMMON_LOG_MEMORY)
    TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TARGET} PUBLIC LKCOMMON_LOG_MEMORY)
    MESSAGE(STATUS "lkCommon: Memory logs ENABLED")
ELSE(LKCOMMON_LOG_MEMORY)
    MESSAGE(STATUS "lkCommon: Memory logs DISABLED")
ENDIF(LKCOMMON_LOG_MEMORY)

IF(LKCOMMON_LOG_CALL)
    TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TARGET} PUBLIC LKCOMMON_LOG_CALL)
    MESSAGE(STATUS "lkCommon: Call logs ENABLED")
ELSE(LKCOMMON_LOG_CALL)
    MESSAGE(STATUS "lkCommon: Call logs DISABLED")
ENDIF(LKCOMMON_LOG_CALL)

ADD_DEPENDENCIES(${LKCOMMON_TARGET} lkCommonDepsPostBuild)

IF(WIN32)
    TARGET_LINK_LIBRARIES(${LKCOMMON_TARGET} PRIVATE zlibstatic png_static)
ELSEIF(UNIX)
    TARGET_LINK_LIBRARIES(${LKCOMMON_TARGET} PRIVATE zlibstatic png_static pthread uuid)
ENDIF(WIN32)

LKCOMMON_COPY_RESULT(${LKCOMMON_TARGET})

# Above TARGET_LINK_LIBRARIES won't actually add symbols to lkCommon.lib, just creates a dependency.
# Actually add dependant static libraries on Windows via lib command.
IF(WIN32)
    IF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        SET(LKCOMMON_DEPS_ZLIB_LIBRARY zlibstaticd.lib)
        SET(LKCOMMON_DEPS_LIBPNG_LIBRARY libpng16_staticd.lib)
    ELSE(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        SET(LKCOMMON_DEPS_ZLIB_LIBRARY zlibstatic.lib)
        SET(LKCOMMON_DEPS_LIBPNG_LIBRARY libpng16_static.lib)
    ENDIF(${CMAKE_BUILD_TYPE} STREQUAL Debug)

    ADD_CUSTOM_COMMAND(TARGET ${LKCOMMON_TARGET} POST_BUILD
                       COMMAND cmake -E rename ${LKCOMMON_TARGET}.lib ${LKCOMMON_TARGET}_tolink.lib
                       COMMAND lib.exe /OUT:${LKCOMMON_TARGET}.lib
                                       ${LKCOMMON_TARGET}_tolink.lib
                                       ${LKCOMMON_DEPS_OUTPUT_DIRECTORY}${LKCOMMON_DEPS_ZLIB_LIBRARY}
                                       ${LKCOMMON_DEPS_OUTPUT_DIRECTORY}${LKCOMMON_DEPS_LIBPNG_LIBRARY}
                                       rpcrt4.lib
                       COMMAND cmake -E remove -f ${LKCOMMON_TARGET}_tolink.lib
                       WORKING_DIRECTORY ${LKCOMMON_OUTPUT_DIRECTORY}
                       COMMENT "Joining lkCommon.lib with static library dependencies" VERBATIM)
ENDIF(WIN32)
