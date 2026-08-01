IF(WIN32)
    SET(LKCOMMON_TEST_PKG_DEPS_LIBRARIES rpcrt4.lib)
ELSEIF(UNIX)
    PKG_CHECK_MODULES(LKCOMMON_TEST_PKG_DEPS REQUIRED
                      xcb
                      xcb-image
                      uuid
                      )
ELSE(WIN32)
    MESSAGE(FATAL_ERROR "Unsupported platform")
ENDIF(WIN32)

SET(LKCOMMON_TEST_SRCS Main.cpp
                       Tests/Allocators/ArenaAllocatorTest.cpp
                       Tests/Allocators/ConcurrentMemoryTest.cpp
                       Tests/Allocators/MemoryObjectTest.cpp
                       Tests/Allocators/MemoryTest.cpp
                       Tests/Allocators/StackAllocatorTest.cpp
                       Tests/Internal/ImageLoaderTest.cpp
                       Tests/Math/AverageTest.cpp
                       Tests/Math/Matrix4Test.cpp
                       Tests/Math/RingAverageTest.cpp
                       Tests/Math/UtilitiesTest.cpp
                       Tests/Math/Vector2Test.cpp
                       Tests/Math/Vector3Test.cpp
                       Tests/Math/Vector4Test.cpp
                       Tests/System/InfoTest.cpp
                       Tests/System/WindowTest.cpp
                       Tests/Utils/ArgParserTest.cpp
                       Tests/Utils/ImageTest.cpp
                       Tests/Utils/LoggerTest.cpp
                       Tests/Utils/PixelTest.cpp
                       Tests/Utils/SortTest.cpp
                       Tests/Utils/StaticQueueTest.cpp
                       Tests/Utils/StaticStackTest.cpp
                       Tests/Utils/StringConvTest.cpp
                       Tests/Utils/ThreadPoolTest.cpp
                       Tests/Utils/TimerTest.cpp
                       Tests/Utils/UUIDTest.cpp
                       )

IF(UNIX)
    SET(LKCOMMON_TEST_LINUX_SRCS Tests/Internal/Linux/XConnectionTest.cpp
                                 )
ENDIF(UNIX)

ADD_EXECUTABLE(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TEST_SRCS}
                                       ${LKCOMMON_TEST_LINUX_SRCS}
                                       )

ADD_DEPENDENCIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET} gtest)

TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TEST_TARGET}
                           PRIVATE -DLKCOMMON_ROOT_DIR="${LKCOMMON_ROOT_DIRECTORY}/"
                           )

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    TARGET_COMPILE_OPTIONS(${LKCOMMON_TEST_TARGET}
                            PRIVATE -std:c++14
                            PRIVATE -arch:SSE2
                            )

    # Inform MSVC about preprocessor defines
    TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TEST_TARGET}
                                PRIVATE _UNICODE
                                PRIVATE _CRT_SECURE_NO_WARNINGS
                                )

    IF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TEST_TARGET}
                               PRIVATE -Od
                               PRIVATE -Zi
                               )
        TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TEST_TARGET}
                                   PRIVATE _DEBUG
                                   )
    ELSE(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TEST_TARGET}
                               PRIVATE -O2
                               PRIVATE -Oi
                               )
    ENDIF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
ELSE(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    TARGET_COMPILE_OPTIONS(${LKCOMMON_TEST_TARGET}
                           PRIVATE -msse4.1
                           PRIVATE -fPIC
                           )

    IF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TEST_TARGET}
                               PRIVATE -O0
                               PRIVATE -ggdb
                               )
        TARGET_COMPILE_DEFINITIONS(${LKCOMMON_TEST_TARGET}
                                   PRIVATE _DEBUG
                                   )
    ELSE(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        TARGET_COMPILE_OPTIONS(${LKCOMMON_TEST_TARGET}
                               PRIVATE -O2
                               )
    ENDIF(${CMAKE_BUILD_TYPE} STREQUAL Debug)
ENDIF(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TEST_TARGET}
                           PRIVATE ${LKCOMMON_DIRECTORY}/include
                           PRIVATE ${LKCOMMON_DIRECTORY}/source/Internal
                           )

TARGET_LINK_LIBRARIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET}
                                              ${LKCOMMON_TEST_PKG_DEPS_LIBRARIES}
                                              gtest
                                              )

LKCOMMON_COPY_RESULT(${LKCOMMON_TEST_TARGET})
