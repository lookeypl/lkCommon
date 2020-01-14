SET(LKCOMMON_SRCS source/Math/CubicInterpolator.cpp
                  source/Math/Interpolator.cpp
                  source/Math/LinearInterpolator.cpp
                  source/Math/Matrix4.cpp
                  source/Math/Utilities.cpp
                  source/Math/Vector4.cpp
                  source/Utils/ArenaAllocator.cpp
                  source/Utils/ArgParser.cpp
                  source/Utils/ImageLoader.cpp
                  source/Utils/ThreadPool.cpp
                  source/Internal/ImageLoaders/PNGImageLoader.cpp
                  )

SET(LKCOMMON_HDRS include/lkCommon/lkCommon.hpp
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
                  include/lkCommon/Math/Vector4.hpp
                  include/lkCommon/Math/Vector4Impl.hpp
                  include/lkCommon/System/FS.hpp
                  include/lkCommon/System/Info.hpp
                  include/lkCommon/System/KeyCodes.hpp
                  include/lkCommon/System/Library.hpp
                  include/lkCommon/System/Memory.hpp
                  include/lkCommon/System/Window.hpp
                  include/lkCommon/System/WindowImage.hpp
                  include/lkCommon/Utils/ArenaAllocator.hpp
                  include/lkCommon/Utils/ArenaObject.hpp
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
                               source/Utils/Win/Timer.cpp
                               source/Utils/Win/StringConv.cpp
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

TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                       PRIVATE "-std=c++1y"
                       PRIVATE "-msse4.1"
                       PRIVATE "-fPIC"
                       )

IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                           PRIVATE "-O0"
                           PRIVATE "-ggdb"
                           )
    TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TARGET}
                               PRIVATE _DEBUG
                               )
ELSE("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    TARGET_COMPILE_OPTIONS(${LKCOMMON_TARGET}
                           PRIVATE "-O2"
                           )
ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

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

TARGET_LINK_LIBRARIES(${LKCOMMON_TARGET} PRIVATE zlibstatic png_static pthread libuuid.a)

LKCOMMON_COPY_RESULT(${LKCOMMON_TARGET})
