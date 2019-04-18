#include "lkCommon/Utils/Image.hpp"


/**
 * Below implementations are empty placeholders to prevent linker errors
 *
 * Windows implementation does not require platform-specific elements to
 * speed up displaying on Window
 */

namespace lkCommon {
namespace System {

WindowImage::WindowImage()
    : mWidth(0)
    , mHeight(0)
    , mDataPtr(nullptr)
{
}

WindowImage::WindowImage(uint32_t width, uint32_t height, void* data)
    : mWidth(width)
    , mHeight(height)
    , mDataPtr(data)
{
}

WindowImage::~WindowImage()
{
}

bool WindowImage::Recreate(uint32_t width, uint32_t height, void* data)
{
    mWidth = width;
    mHeight = height;
    mDataPtr = data;
    return true;
}

void* WindowImage::GetHandle() const
{
    return mDataPtr;
}

} // namespace System
} // namespace lkCommon
