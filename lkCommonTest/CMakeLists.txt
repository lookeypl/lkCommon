PKG_CHECK_MODULES(LKCOMMON_TEST_PKG_DEPS REQUIRED
                  xcb
                  xcb-image
                  )

SET(LKCOMMON_TEST_SRCS Main.cpp
                       Tests/Math/RingAverageTest.cpp
                       Tests/Math/Vector4Test.cpp
                       Tests/System/WindowTest.cpp
                       Tests/Utils/ImageTest.cpp
                       Tests/Utils/LoggerTest.cpp
                       Tests/Utils/PixelTest.cpp
                       Tests/Utils/SortTest.cpp
                       Tests/Utils/StringConvTest.cpp
                       Tests/Utils/ThreadPoolTest.cpp
                       )

ADD_EXECUTABLE(${LKCOMMON_TEST_TARGET}
               ${LKCOMMON_TEST_SRCS}
               )

ADD_DEPENDENCIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET} gtest)

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TEST_TARGET}
                           PUBLIC ${LKCOMMON_DIRECTORY}/include
                           )

TARGET_LINK_LIBRARIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET}
                      ${LKCOMMON_TEST_PKG_DEPS_LIBRARIES}
                      gtest
                      )

LKCOMMON_COPY_RESULT(${LKCOMMON_TEST_TARGET})
