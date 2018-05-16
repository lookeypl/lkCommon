#pragma once

#include <cstdint>
#include <vector>


namespace lkCommon {

/**
 * A basic Image, which can be filled with whatever data is required
 */
class Image final
{
    friend class Window;

    struct Pixel
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;

        Pixel()
            : b(0), g(0), r(0)
        {
        }

        Pixel(uint8_t r, uint8_t g, uint8_t b)
            : b(b), g(g), r(r)
        {
        }
    };

    int mWidth;
    int mHeight;
    std::vector<Pixel> mPixels;

    size_t GetPixelCoord(uint32_t x, uint32_t y);

public:
    Image();
    ~Image();

    bool Resize(uint32_t width, uint32_t height);
    void SetPixel(uint32_t x, uint32_t y, const Pixel& pixel);
    void SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
};

} // namespace lkCommon
