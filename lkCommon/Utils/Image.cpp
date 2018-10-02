/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Image module implementation
 */

#include "Image.hpp"
#include "Logger.hpp"


namespace {

// swaps B and R components to adjust them for destination media
template <typename T>
lkCommon::Utils::Pixel<T, 4> FixColorComponents(const lkCommon::Utils::Pixel<T, 4>& p)
{
    T pixelRaw[4] = { p.mColors[2], p.mColors[1], p.mColors[0], p.mColors[3] };
    return lkCommon::Utils::Pixel<T, 4>(pixelRaw);
}

} // namespace


namespace lkCommon {
namespace Utils {

Image::Image()
    : mWidth(0)
    , mHeight(0)
    , mPixels()
{
}

Image::Image(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
    , mPixels(mWidth * mHeight)
{
}

Image::~Image()
{
}

size_t Image::GetPixelCoord(uint32_t x, uint32_t y)
{
    if (x >= mWidth || y >= mHeight)
    {
        LOGE("Requested pixel coordinates (" << x << ", " << y << ") extend too far - " <<
             "limits (" << mWidth << ", " << mHeight << ") shouldn't be met, or crossed.");
        return SIZE_MAX;
    }

    return y * mWidth + x;
}

bool Image::Resize(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;

    try {
        mPixels.resize(mWidth * mHeight);
    } catch (std::exception& e) {
        LOGE("Failed to resize Image: " << e.what());
        return false;
    }

    return true;
}

bool Image::SetPixel(uint32_t x, uint32_t y, const Pixel<uint8_t, 4>& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        return false;
    }

    mPixels[coord] = FixColorComponents(pixel);
    return true;
}

bool Image::GetPixel(uint32_t x, uint32_t y, Pixel<uint8_t, 4>& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        return false;
    }

    pixel = FixColorComponents(mPixels[coord]);
    return true;
}

} // namespace Utils
} // namespace lkCommon
