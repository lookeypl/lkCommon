#include "lkCommon/Utils/Image.hpp"

#include "Linux/XConnection.hpp"


namespace lkCommon {
namespace Utils {

void Image::InitPlatformSpecific()
{
    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    xcb_screen_t* screen = Internal::XConnection::Instance().GetScreen();
    if (connection == nullptr || screen == nullptr)
    {
        LOGE("Failed to connect to X server");
        return;
    }

    const uint8_t* data = reinterpret_cast<const uint8_t*>(mPixels.data());
    mXcbImage = xcb_image_create_native(connection,
                                        mWidth, mHeight,
                                        XCB_IMAGE_FORMAT_Z_PIXMAP,
                                        screen->root_depth, nullptr,
                                        mWidth * mHeight * sizeof(Utils::Pixel<uint8_t, 4>),
                                        const_cast<uint8_t*>(data));
    if (mXcbImage == nullptr)
    {
        LOGE("Failed to create XCB image");
    }
}

bool Image::ResizePlatformSpecific()
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

    const uint8_t* data = reinterpret_cast<const uint8_t*>(mPixels.data());
    mXcbImage = xcb_image_create_native(connection,
                                        mWidth, mHeight,
                                        XCB_IMAGE_FORMAT_Z_PIXMAP,
                                        screen->root_depth, nullptr,
                                        mWidth * mHeight * sizeof(Utils::Pixel<uint8_t, 4>),
                                        const_cast<uint8_t*>(data));
    if (mXcbImage == nullptr)
    {
        LOGE("Failed to create XCB image");
        return false;
    }

    return true;
}

void Image::ReleasePlatformSpecific()
{
    if (mXcbImage != nullptr)
    {
        xcb_image_destroy(mXcbImage);
    }
}

void* Image::GetPlatformImageHandle() const
{
    return reinterpret_cast<void*>(mXcbImage);
}

} // namespace Utils
} // namespace lkCommon