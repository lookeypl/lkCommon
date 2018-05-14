SET(LKCOMMON_SRCS PCH.cpp
                  )

SET(LKCOMMON_HDRS FS.hpp
                  KeyCodes.hpp
                  Library.hpp
                  lkCommon.hpp
                  Logger.hpp
                  PCH.hpp
                  Timer.hpp
                  UTF.hpp
                  Window.hpp
                  )

IF(WIN32)
    SET(LKCOMMON_PLATFORM_SRCS Win/FS.cpp
                               Win/Library.cpp
                               Win/Logger.cpp
                               Win/Timer.cpp
                               Win/UTF.cpp
                               Win/Window.cpp
                               )
ELSEIF(UNIX)
    SET(LKCOMMON_PLATFORM_SRCS Linux/FS.cpp
                               Linux/Library.cpp
                               Linux/Logger.cpp
                               Linux/Timer.cpp
                               Linux/UTF.cpp
                               Linux/Window.cpp
                               )
ELSE()
    MESSAGE(FATAL_ERROR "Platform unsupported")
ENDIF()

ADD_LIBRARY(${LKCOMMON_TARGET}
            ${LKCOMMON_SRCS} ${LKCOMMON_HDRS}
            ${LKCOMMON_PLATFORM_SRCS}
            )

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TARGET} PRIVATE ${LKCOMMON_DIRECTORY})

LKCOMMON_COPY_RESULT(${LKCOMMON_TARGET})
