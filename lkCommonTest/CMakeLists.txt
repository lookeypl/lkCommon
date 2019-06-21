PKG_CHECK_MODULES(LKCOMMON_TEST_PKG_DEPS REQUIRED
                  xcb
                  xcb-image
                  )

SET(LKCOMMON_TEST_SRCS Main.cpp
                       Tests/Internal/Linux/XConnectionTest.cpp
                       Tests/Internal/ImageLoaderTest.cpp
                       Tests/Math/AverageTest.cpp
                       Tests/Math/Matrix4Test.cpp
                       Tests/Math/RingAverageTest.cpp
                       Tests/Math/UtilitiesTest.cpp
                       Tests/Math/Vector4Test.cpp
                       Tests/System/InfoTest.cpp
                       Tests/System/WindowTest.cpp
                       Tests/Utils/ArenaAllocatorTest.cpp
                       Tests/Utils/ArenaObjectTest.cpp
                       Tests/Utils/ArgParserTest.cpp
                       Tests/Utils/ImageTest.cpp
                       Tests/Utils/LoggerTest.cpp
                       Tests/Utils/PixelTest.cpp
                       Tests/Utils/SortTest.cpp
                       Tests/Utils/StaticQueueTest.cpp
                       Tests/Utils/StaticStackTest.cpp
                       Tests/Utils/StringConvTest.cpp
                       Tests/Utils/ThreadPoolTest.cpp
                       )

ADD_EXECUTABLE(${LKCOMMON_TEST_TARGET}
               ${LKCOMMON_TEST_SRCS}
               )

ADD_DEPENDENCIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET} gtest)

ADD_DEFINITIONS(-msse4.1 -fPIC)
ADD_DEFINITIONS(-DLKCOMMON_ROOT_DIR="${LKCOMMON_ROOT_DIRECTORY}/")

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TEST_TARGET}
                           PUBLIC ${LKCOMMON_DIRECTORY}/include
                           PUBLIC ${LKCOMMON_DIRECTORY}/source/Internal
                           )

TARGET_LINK_LIBRARIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET}
                      ${LKCOMMON_TEST_PKG_DEPS_LIBRARIES}
                      gtest
                      )

LKCOMMON_COPY_RESULT(${LKCOMMON_TEST_TARGET})
