#pragma once

#include <cstdint>
#include <vector>

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace lkCommon {
namespace Utils {

/**
 * A basic Image, which can be filled with whatever data is required
 */
class Image final
{
    uint32_t mWidth;
    uint32_t mHeight;
    std::vector<Pixel<uint8_t, 4>> mPixels;

    size_t GetPixelCoord(uint32_t x, uint32_t y);

public:
    Image();
    ~Image();

    bool Resize(uint32_t width, uint32_t height);
    bool SetPixel(uint32_t x, uint32_t y, const Pixel<uint8_t, 4>& pixel);
    bool SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    bool SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    bool GetPixel(uint32_t x, uint32_t y, Pixel<uint8_t, 4>& pixel);
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

    LKCOMMON_INLINE const Pixel<uint8_t, 4>* GetDataPtr() const
    {
        return mPixels.data();
    }
};

} // namespace Utils
} // namespace lkCommon
