/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Image module implementation
 */

#pragma once

#include <cstdint>
#include <vector>

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Utils/Pixel.hpp>

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#elif defined(__linux__) | defined(__LINUX__)
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#else
#error "Platform not supported"
#endif


namespace lkCommon {
namespace Utils {

/**
 * A basic Image, which can be filled with whatever data is required.
 *
 * Image class is made as an utility allowing user to draw any contents on it
 * and display it on Window type object. Data is stored in one-dimensional
 * array using std::vector as a container.
 *
 * @remarks List of ideas / TODOs:
 *  - Image should be converted into a template with arguments passed to Pixel
 *  - Image could have ex. a specialization <uint8_t, 4> used specifically with
 *    Window to keep data in an appropriate format for Window class
 */
class Image final
{
    uint32_t mWidth;
    uint32_t mHeight;
    std::vector<PixelUint4> mPixels;

#ifdef WIN32
    // empty, Windows implementation does not require platform-specific code
#elif defined(__linux__) | defined(__LINUX__)
    xcb_image_t* mXcbImage;
#else
#error "Platform not supported"
#endif

    size_t GetPixelCoord(uint32_t x, uint32_t y);
    void InitPlatformSpecific();
    bool ResizePlatformSpecific();
    void ReleasePlatformSpecific();

public:
    /**
     * Constructs a default empty Image. To fit any data, Resize() must be
     * called after construction.
     */
    Image();

    /**
     * Constructs an Image with dimensions @p width x @p height.
     *
     * @p[in] width  Width of image
     * @p[in] height Height of image
     *
     * @note In case of error (ex. not enough memory) constructor may throw.
     * Possible thrown exceptions match std::vector::resize() exceptions.
     */
    Image(uint32_t width, uint32_t height);

    /**
     * Destroys Image object, freeing all allocated memory.
     */
    ~Image();

    /**
     * Resizes an image to fit @p width x @p height pixels.
     *
     * @p[in] width  New width of Image.
     * @p[in] height New height of Image.
     * @result True if resizing succeeded, false if there's no memory left.
     */
    bool Resize(uint32_t width, uint32_t height);

    /**
     * Sets pixel at position @p x and @p y to value @p pixel.
     *
     * @p[in] x     X coordinate of Pixel to modify. Fits in range [0; mWidth).
     * @p[in] y     Y coordinate of Pixel to modify. Fits in range [0; mHeight).
     * @p[in] pixel Pixel value to set on coordinates @p x and @p y.
     * @result True if succeeds, false when x or y are out of bounds.
     */
    bool SetPixel(uint32_t x, uint32_t y, const Pixel<uint8_t, 4>& pixel);

    /**
     * Gets pixel at position @p x and @p y and stores its value in @p pixel.
     *
     * @p[in]  x     X coordinate of Pixel to acquire. Fits in range [0; mWidth).
     * @p[in]  y     Y coordinate of Pixel to acquire. Fits in range [0; mHeight).
     * @p[out] pixel Pixel value on coordinates @p x and @p y.
     * @result True if succeeds, false when x or y are out of bounds.
     */
    bool GetPixel(uint32_t x, uint32_t y, Pixel<uint8_t, 4>& pixel);

    /**
     * Acquires platform-specific bitmap handle.
     *
     * @note This function should be used ONLY by platform-specific modules.
     * Result can be easily casted there to:
     *   - HBITMAP on Windows
     *   - xcb_image* on Linux
     */
    void* GetPlatformImageHandle() const;

    /**
     * Returns width of Image.
     */
    LKCOMMON_INLINE uint32_t GetWidth() const
    {
        return mWidth;
    }

    /**
     * Returns height of Image.
     */
    LKCOMMON_INLINE uint32_t GetHeight() const
    {
        return mHeight;
    }

    /**
     * Returns pointer to data array used by Pixel.
     */
    LKCOMMON_INLINE const Pixel<uint8_t, 4>* GetDataPtr() const
    {
        return mPixels.data();
    }
};

} // namespace Utils
} // namespace lkCommon
