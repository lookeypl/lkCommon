#include "lkCommon/Utils/Image.hpp"


namespace lkCommon {
namespace Utils {

void Image::InitPlatformSpecific()
{
    // TODO globalize
    mConnection = xcb_connect(nullptr, &mConnScreen);
    if (mConnection == nullptr)
    {
        LOGE("Failed to connect to X server");
        return;
    }

    const xcb_setup_t* xcbSetup;
    xcb_screen_iterator_t xcbScreenIt;

    xcbSetup = xcb_get_setup(mConnection);
    xcbScreenIt = xcb_setup_roots_iterator(xcbSetup);
    while (mConnScreen-- > 0)
        xcb_screen_next(&xcbScreenIt);
    mScreen = xcbScreenIt.data;

    const uint8_t* data = reinterpret_cast<const uint8_t*>(mPixels.data());
    mXcbImage = xcb_image_create_native(mConnection,
                                        mWidth, mHeight,
                                        XCB_IMAGE_FORMAT_Z_PIXMAP,
                                        mScreen->root_depth, nullptr,
                                        mWidth * mHeight * sizeof(Utils::Pixel<uint8_t, 4>),
                                        const_cast<uint8_t*>(data));
    if (mXcbImage == nullptr)
    {
        LOGE("Failed to create XCB image");
    }
}

bool Image::ResizePlatformSpecific()
{
    if (mXcbImage != nullptr)
    {
        xcb_image_destroy(mXcbImage);
    }

    const uint8_t* data = reinterpret_cast<const uint8_t*>(mPixels.data());
    mXcbImage = xcb_image_create_native(mConnection,
                                        mWidth, mHeight,
                                        XCB_IMAGE_FORMAT_Z_PIXMAP,
                                        mScreen->root_depth, nullptr,
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

    if (mConnection != nullptr)
    {
        xcb_disconnect(mConnection);
    }
}

void* Image::GetPlatformImageHandle() const
{
    return reinterpret_cast<void*>(mXcbImage);
}

} // namespace Utils
} // namespace lkCommon