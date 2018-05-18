#include "PCH.hpp"
#include "Image.hpp"
#include "Logger.hpp"


namespace lkCommon {

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
        LOGE("Requested pixel coordinates (" << x << ", " << y << ") extend too far - limits are (" << mWidth << ", " << mHeight << ")");
        return SIZE_MAX;
    }

    return y * mHeight + x;
}

bool Image::Resize(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;
    mPixels.resize(mWidth * mHeight);
    return true;
}

void Image::SetPixel(uint32_t x, uint32_t y, const Image::Pixel& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        LOGE("Cannot set pixel value - invalid coordinates");
        return;
    }

    mPixels[coord] = pixel;
}

void Image::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    Pixel p(r, g, b);
    SetPixel(x, y, p);
}

void Image::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    Pixel p(r, g, b, a);
    SetPixel(x, y, p);
}

} // namespace lkCommon
