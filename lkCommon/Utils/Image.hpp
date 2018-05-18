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

    uint32_t mWidth;
    uint32_t mHeight;
    std::vector<Pixel> mPixels;

    size_t GetPixelCoord(uint32_t x, uint32_t y);

public:
    Image();
    ~Image();

    bool Resize(uint32_t width, uint32_t height);
    void SetPixel(uint32_t x, uint32_t y, const Pixel& pixel);
    void SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    void SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

} // namespace lkCommon
