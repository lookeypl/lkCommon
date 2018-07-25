#pragma once

#include <cstdint>
#include <vector>

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace lkCommon {
namespace Utils {

/**
 * A basic Image, which can be filled with whatever data is required
 *
 * Ideas / TODO:
 *  - Image should be converted into a template with arguments passed to Pixel
 *  - Image could have ex. a specialization <uint8_t, 4> used specifically with Window to keep data
 *    in an appropriate format for Window class
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
    bool GetPixel(uint32_t x, uint32_t y, Pixel<uint8_t, 4>& pixel);

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
