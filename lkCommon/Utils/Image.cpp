#include "Image.hpp"
#include "Logger.hpp"


namespace lkCommon {
namespace Utils {

Image::Image()
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
    mPixels.resize(mWidth * mHeight);
    return true;
}

bool Image::SetPixel(uint32_t x, uint32_t y, const Pixel<uint8_t, 4>& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        return false;
    }

    mPixels[coord] = pixel;
    return true;
}

bool Image::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t pixels[] = {r, g, b, 0};
    return SetPixel(x, y, Pixel<uint8_t, 4>(pixels));
}

bool Image::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint8_t pixels[] = {r, g, b, a};
    return SetPixel(x, y, Pixel<uint8_t, 4>(pixels));
}

bool Image::GetPixel(uint32_t x, uint32_t y, Pixel<uint8_t, 4>& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        return false;
    }

    pixel = mPixels[coord];
    return true;
}

bool Image::GetPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b)
{
    Pixel<uint8_t, 4> p;
    if (!GetPixel(x, y, p))
        return false;

    r = p.mColors[2];
    g = p.mColors[1];
    b = p.mColors[0];
    return true;
}

bool Image::GetPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a)
{
    Pixel<uint8_t, 4> p;
    if (!GetPixel(x, y, p))
        return false;

    r = p.mColors[2];
    g = p.mColors[1];
    b = p.mColors[0];
    a = p.mColors[3];
    return true;
}

} // namespace Utils
} // namespace lkCommon
