#pragma once

#if !defined(__linux__) && !defined(__LINUX__)
#error "This header should NOT be included on other platforms than Linux"
#endif

#include <lkCommon/lkCommon.hpp>
#include <xcb/xcb.h>


namespace lkCommon {
namespace Internal {

class XConnection
{
    xcb_connection_t* mConnection;
    xcb_screen_t* mScreen;
    int mConnScreen;

    XConnection();
    ~XConnection();

public:
    static XConnection& Instance();

    LKCOMMON_INLINE xcb_connection_t* GetConnection()
    {
        return mConnection;
    }

    LKCOMMON_INLINE xcb_screen_t* GetScreen()
    {
        return mScreen;
    }
};

} // namespace lkCommon
} // namespace Internal