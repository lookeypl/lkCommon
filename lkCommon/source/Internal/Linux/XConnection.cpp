#include "XConnection.hpp"

namespace lkCommon {
namespace Internal {

XConnection::XConnection()
    : mConnection(nullptr)
    , mScreen(nullptr)
{
    mConnection = xcb_connect(nullptr, &mConnScreen);
    if (xcb_connection_has_error(mConnection))
    {
        LOGE("Failed to connect to X server through XCB");
        mConnection = nullptr;
        return;
    }

    const xcb_setup_t* xcbSetup;
    xcb_screen_iterator_t xcbScreenIt;

    xcbSetup = xcb_get_setup(mConnection);
    xcbScreenIt = xcb_setup_roots_iterator(xcbSetup);
    while (mConnScreen-- > 0)
        xcb_screen_next(&xcbScreenIt);
    mScreen = xcbScreenIt.data;
}

XConnection::~XConnection()
{
    if (mConnection)
    {
        xcb_flush(mConnection);
        xcb_disconnect(mConnection);
    }
}

XConnection& XConnection::Instance()
{
    static XConnection instance;
    return instance;
}

} // namespace Internal
} // namespace lkCommon
