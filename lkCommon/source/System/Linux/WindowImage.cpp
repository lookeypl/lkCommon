#include "lkCommon/Utils/Image.hpp"

#include "Linux/XConnection.hpp"


namespace lkCommon {
namespace System {

WindowImage::WindowImage()
    : mWidth(0)
    , mHeight(0)
    , mXcbImage(nullptr)
{
}

WindowImage::WindowImage(uint32_t width, uint32_t height, void* data)
    : mWidth(width)
    , mHeight(height)
    , mXcbImage(nullptr)
{
    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    xcb_screen_t* screen = Internal::XConnection::Instance().GetScreen();
    if (connection == nullptr || screen == nullptr)
    {
        LOGE("Failed to connect to X server");
        return;
    }

    mXcbImage = xcb_image_create_native(connection,
                                        mWidth, mHeight,
                                        XCB_IMAGE_FORMAT_Z_PIXMAP,
                                        screen->root_depth, nullptr,
                                        mWidth * mHeight * sizeof(Utils::Pixel<uint8_t, 4>),
                                        reinterpret_cast<uint8_t*>(data));
    if (mXcbImage == nullptr)
    {
        LOGE("Failed to create XCB image");
    }
}

WindowImage::~WindowImage()
{
    if (mXcbImage != nullptr)
    {
        xcb_image_destroy(mXcbImage);
    }
}

bool WindowImage::Recreate(uint32_t width, uint32_t height, void* data)
{
    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    xcb_screen_t* screen = Internal::XConnection::Instance().GetScreen();
    if (connection == nullptr || screen == nullptr)
    {
        LOGE("Failed to connect to X server");
        return false;
    }

    if (mXcbImage != nullptr)
    {
        xcb_image_destroy(mXcbImage);
    }

    mWidth = width;
    mHeight = height;
    mXcbImage = xcb_image_create_native(connection,
                                        mWidth, mHeight,
                                        XCB_IMAGE_FORMAT_Z_PIXMAP,
                                        screen->root_depth, nullptr,
                                        mWidth * mHeight * sizeof(Utils::Pixel<uint8_t, 4>),
                                        reinterpret_cast<uint8_t*>(data));
    if (mXcbImage == nullptr)
    {
        LOGE("Failed to create XCB image");
        return false;
    }

    return true;
}

void* WindowImage::GetHandle() const
{
    return reinterpret_cast<void*>(mXcbImage);
}

} // namespace System
} // namespace lkCommon