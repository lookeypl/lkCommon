#pragma once

#include <cstdint>
#include <vector>

#include <lkCommon.hpp>


namespace lkCommon {
namespace Utils {

/**
 * A basic Image, which can be filled with whatever data is required
 */
class Image final
{
public:
    struct Pixel
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;

        Pixel()
            : b(0), g(0), r(0), a(0)
        {
        }

        Pixel(uint8_t r, uint8_t g, uint8_t b)
            : b(b), g(g), r(r), a(0)
        {
        }

        Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
            : b(b), g(g), r(r), a(a)
        {
        }
    };

private:
    uint32_t mWidth;
    uint32_t mHeight;
    std::vector<Pixel> mPixels;

    size_t GetPixelCoord(uint32_t x, uint32_t y);

public:
    Image();
    ~Image();

    bool Resize(uint32_t width, uint32_t height);
    bool SetPixel(uint32_t x, uint32_t y, const Pixel& pixel);
    bool SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    bool SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    bool GetPixel(uint32_t x, uint32_t y, Image::Pixel& pixel);
    bool GetPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b);
    bool GetPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

    LKCOMMON_INLINE uint32_t GetWidth() const
    {
        return mWidth;
    }

    LKCOMMON_INLINE uint32_t GetHeight() const
    {
        return mHeight;
    }

    LKCOMMON_INLINE const Pixel* GetDataPtr() const
    {
        return mPixels.data();
    }
};

} // namespace Utils
} // namespace lkCommon
