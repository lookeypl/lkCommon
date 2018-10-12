SET(LKCOMMON_SRCS source/Math/Vector4.cpp
                  source/Math/Interpolator.cpp
                  source/Math/LinearInterpolator.cpp
                  source/Math/CubicInterpolator.cpp
                  source/Utils/Image.cpp
                  source/Utils/ThreadPool.cpp
                  )

SET(LKCOMMON_HDRS include/lkCommon/lkCommon.hpp
                  include/lkCommon/Math/Constants.hpp
                  include/lkCommon/Math/RingAverage.hpp
                  include/lkCommon/Math/RingAverageImpl.hpp
                  include/lkCommon/Math/Vector4.hpp
                  include/lkCommon/Math/Interpolator.hpp
                  include/lkCommon/Math/LinearInterpolator.hpp
                  include/lkCommon/Math/CubicInterpolator.hpp
                  include/lkCommon/System/FS.hpp
                  include/lkCommon/System/Info.hpp
                  include/lkCommon/System/KeyCodes.hpp
                  include/lkCommon/System/Library.hpp
                  include/lkCommon/System/Window.hpp
                  include/lkCommon/Utils/Image.hpp
                  include/lkCommon/Utils/Logger.hpp
                  include/lkCommon/Utils/Sort.hpp
                  include/lkCommon/Utils/SortImpl.hpp
                  include/lkCommon/Utils/ThreadPool.hpp
                  include/lkCommon/Utils/Timer.hpp
                  include/lkCommon/Utils/StringConv.hpp
                  )



IF(WIN32)
    SET(LKCOMMON_PLATFORM_SRCS source/System/Win/FS.cpp
                               source/System/Win/Library.cpp
                               source/System/Win/Window.cpp
                               source/Utils/Win/ImageWin.cpp
                               source/Utils/Win/Logger.cpp
                               source/Utils/Win/Timer.cpp
                               source/Utils/Win/StringConv.cpp
                               )
ELSEIF(UNIX)
    SET(LKCOMMON_PLATFORM_SRCS source/System/Linux/FS.cpp
                               source/System/Linux/Info.cpp
                               source/System/Linux/Library.cpp
                               source/System/Linux/Window.cpp
                               source/Utils/Linux/ImageLinux.cpp
                               source/Utils/Linux/Logger.cpp
                               source/Utils/Linux/Timer.cpp
                               source/Utils/Linux/StringConv.cpp
                               )
ELSE()
    MESSAGE(FATAL_ERROR "Platform unsupported")
ENDIF()

ADD_DEFINITIONS(-msse4.1 -fPIC)

ADD_LIBRARY(${LKCOMMON_TARGET}
            ${LKCOMMON_SRCS} ${LKCOMMON_HDRS}
            ${LKCOMMON_PLATFORM_SRCS}
            )

TARGET_INCLUDE_DIRECTORIES(${LKCOMMON_TARGET}
                           PRIVATE ${LKCOMMON_DIRECTORY}/include/
                           )

TARGET_LINK_LIBRARIES(${LKCOMMON_TARGET} pthread)

LKCOMMON_COPY_RESULT(${LKCOMMON_TARGET})
