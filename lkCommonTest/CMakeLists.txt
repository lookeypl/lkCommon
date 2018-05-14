PKG_CHECK_MODULES(LKCOMMON_TEST_PKG_DEPS REQUIRED
                  xcb
                  )

SET(LKCOMMON_TEST_SRCS Main.cpp
                       Tests/LoggerTest.cpp
                       Tests/WindowTest.cpp
                       )

ADD_EXECUTABLE(${LKCOMMON_TEST_TARGET}
               ${LKCOMMON_TEST_SRCS}
               )

ADD_DEPENDENCIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET} gtest)

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TEST_TARGET}
                           PUBLIC ${LKCOMMON_ROOT_DIRECTORY}
                           )

TARGET_LINK_LIBRARIES(${LKCOMMON_TEST_TARGET} ${LKCOMMON_TARGET}
                      ${LKCOMMON_TEST_PKG_DEPS_LIBRARIES}
                      gtest
                      )

LKCOMMON_COPY_RESULT(${LKCOMMON_TEST_TARGET})
