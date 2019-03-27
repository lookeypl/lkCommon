/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Image template class declaration
 */

#pragma once
#define _LKCOMMON_UTILS_IMAGE_HPP_

#include <cstdint>
#include <vector>

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/System/WindowImage.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace lkCommon {
namespace Utils {

enum class Sampling: unsigned char
{
    NEAREST = 0,
    BILINEAR,
};

/**
 * A basic Image, which can be filled with whatever data is required.
 *
 * Image class is made as an utility allowing user to draw any contents on it
 * and display it on Window type object. Data is stored in one-dimensional
 * array using std::vector as a container.
 */
template <typename PixelType>
class Image final
{
public:
    using PixelContainer = std::vector<PixelType>;

private:
    uint32_t mWidth;
    uint32_t mHeight;
    float mWidthStep;
    float mHeightStep;
    PixelContainer mPixels;
    System::WindowImage mWindowImage;

    size_t GetPixelCoord(uint32_t x, uint32_t y);
    PixelType SampleNearest(float x, float y);
    PixelType SampleBilinear(float x, float y);

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
     * Constructs an image with dimensions @p width x @p height and fills it
     * with data coming from @p data.
     *
     * Image data in @p data will be copied to the object. If there's less data
     * than @p width x @p height, rest will be filled with zeros. If there's more
     * data, it will be omitted. Data is assumed to be delivered in row order.
     *
     * @p[in] width         Width of image
     * @p[in] height        Height of image
     * @p[in] pixelsPerRow  Amount of pixels in one row of @p data. Can be 0 - then
                            constructor assumes row width equal to @p width.
     * @p[in] data          Data for image to be filled with.
     * @p[in] isBGR         True if provided pixel data is in RGB format.
     *
     * @note In case of error (ex. not enough memory) constructor may throw.
     * Possible thrown exceptions match std::vector::resize() exceptions.
     */
    Image(uint32_t width, uint32_t height, uint32_t pixelsPerRow, const PixelContainer& data, bool isBGR = false);

    Image(const Image<PixelType>& other);
    Image(Image<PixelType>&& other);
    Image& operator=(const Image<PixelType>& other);
    Image& operator=(Image<PixelType>&& other);

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
    bool SetPixel(uint32_t x, uint32_t y, const PixelType& pixel);

    /**
     * Gets pixel at position @p x and @p y and stores its value in @p pixel.
     *
     * @p[in]  x     X coordinate of Pixel to acquire. Fits in range [0; mWidth).
     * @p[in]  y     Y coordinate of Pixel to acquire. Fits in range [0; mHeight).
     * @p[out] pixel Pixel value on coordinates @p x and @p y.
     * @result True if succeeds, false when x or y are out of bounds.
     */
    bool GetPixel(uint32_t x, uint32_t y, PixelType& pixel);

    /**
     * Sets all pixels into one color
     */
    void SetAllPixels(const PixelType& color);

    /**
     * Samples image at coordinates x and y using requested sampling method.
     *
     * @p[in] x            X coordinate of image in range [0..1]
     * @p[in] y            Y coordinate of image in range [0..1]
     * @p[in] samplingType Type of sampling to use
     * @result Sampled pixel color at given coordinates.
     *
     * @warning Some Pixel types might not return correct sampling results on
     * some sampling types. Approximation-related methods like Bilinear sampling
     * can suffer from approximation errors, or overflows. For best results,
     * do NOT use this function with integer-based types like PixelUint4.
     */
    PixelType Sample(float x, float y, Sampling samplingType);

    /**
     * Cast operator between Pixel types.
     *
     * @note Casting requires PixelType -> ConvType static conversion to be possible.
     */
    template <typename ConvType>
    operator Image<ConvType>() const;

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
    LKCOMMON_INLINE const PixelType* GetDataPtr() const
    {
        return mPixels.data();
    }

    /**
     * Returns WindowImage to use for displaying
     */
    LKCOMMON_INLINE const System::WindowImage& GetWindowImage() const
    {
        return mWindowImage;
    }
};

} // namespace Utils
} // namespace lkCommon

#include "ImageImpl.hpp"
