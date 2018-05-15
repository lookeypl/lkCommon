SET(LKCOMMON_SRCS PCH.cpp
                  Math/Vector4.cpp
                  Utils/Image.cpp
                  )

SET(LKCOMMON_HDRS lkCommon.hpp
                  PCH.hpp
                  Math/Vector4.hpp
                  System/FS.hpp
                  System/KeyCodes.hpp
                  System/Window.hpp
                  Utils/Image.hpp
                  Utils/Library.hpp
                  Utils/Logger.hpp
                  Utils/Timer.hpp
                  Utils/UTF.hpp
                  )



IF(WIN32)
    SET(LKCOMMON_PLATFORM_SRCS System/Win/FS.cpp
                               System/Win/Window.cpp
                               Utils/Win/Library.cpp
                               Utils/Win/Logger.cpp
                               Utils/Win/Timer.cpp
                               Utils/Win/UTF.cpp
                               )
ELSEIF(UNIX)
    SET(LKCOMMON_PLATFORM_SRCS System/Linux/FS.cpp
                               System/Linux/Window.cpp
                               Utils/Linux/Library.cpp
                               Utils/Linux/Logger.cpp
                               Utils/Linux/Timer.cpp
                               Utils/Linux/UTF.cpp
                               )
ELSE()
    MESSAGE(FATAL_ERROR "Platform unsupported")
ENDIF()

ADD_DEFINITIONS(-msse4.1)

ADD_LIBRARY(${LKCOMMON_TARGET}
            ${LKCOMMON_SRCS} ${LKCOMMON_HDRS}
            ${LKCOMMON_PLATFORM_SRCS}
            )

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TARGET}
                           PRIVATE ${LKCOMMON_DIRECTORY}
                           PRIVATE ${LKCOMMON_ROOT_DIRECTORY}
                           )

LKCOMMON_COPY_RESULT(${LKCOMMON_TARGET})
