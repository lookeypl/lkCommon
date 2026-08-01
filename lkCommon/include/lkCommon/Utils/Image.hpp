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
    using MipmapContainer = std::vector<PixelContainer>;

private:
    uint32_t mWidth;
    uint32_t mHeight;
    PixelContainer mPixels;
    MipmapContainer mMipmaps;
    System::WindowImage mWindowImage;

    // unwrapped version - returns SIZE_MAX when bounds are crossed
    size_t GetPixelCoord(uint32_t x, uint32_t y) const;

    // wrapped version - crossing the boundaries will subtract width/height
    // to go back in bounds
    size_t GetPixelCoordWrapped(uint32_t x, uint32_t y) const;

    // nearest-neighbor sampler
    PixelType SampleNearest(float x, float y) const;

    // bilinear sampler
    PixelType SampleBilinear(float x, float y) const;

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

    /**
     * Constructs an image from file provided by @p path.
     *
     * @p[in] path Path to file to be loaded.
     */
    Image(const std::string& path);

    Image(const Image<PixelType>& other);
    Image(Image<PixelType>&& other);
    Image& operator=(const Image<PixelType>& other);
    Image& operator=(Image<PixelType>&& other);

    /**
     * Destroys Image object, freeing all allocated memory.
     */
    ~Image();

    /**
     * Loads image from file. Image type is determined automatically.
     *
     * @p[in] path Path to image to load
     * @result True if loading succeeded, false on error.
     */
    bool Load(const std::string& path);

    /**
     * Resizes an image to fit @p width x @p height pixels. Does nothing if
     * both new width and new height are equal to current.
     *
     * @p[in] width  New width of Image.
     * @p[in] height New height of Image.
     * @result True if resizing succeeded, false if there's no memory left or
     *         arguments are invalid.
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
     * @p[in] x            X coordinate of image
     * @p[in] y            Y coordinate of image
     * @p[in] samplingType Type of sampling to use
     * @result Sampled pixel color at given coordinates.
     *
     * @note X and Y coordinates wrap around (ex. sampling at X = 1.5f gives the
     * same result as sampling at X = 0.5, X = 2.5f etc).
     *
     * @warning Some Pixel types might not return correct sampling results on
     * some sampling types. Approximation-related methods like Bilinear sampling
     * can suffer from approximation errors, or overflows. For best results,
     * do NOT use this function with integer-based types like PixelUint4.
     */
    PixelType Sample(float x, float y, Sampling samplingType) const;

    /**
     * Rescales an image to given width/height.
     *
     * This function produces new Image object and leaves contents of current one intact.
     */
    Image<PixelType> Rescale(uint32_t width, uint32_t height, Sampling samplingType) const;

    /**
     * Generate mipmaps for Image.
     */
    void GenerateMipmaps(Sampling samplingType);

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

    /**
     * Returns Pixels (a vector of PixelType objects)
     */
    LKCOMMON_INLINE const PixelContainer& GetPixels() const
    {
        return mPixels;
    }

    /**
     * Returns Mipmaps (a vector of PixelContainers)
     *
     * @note Mipmaps returned here do NOT include base image! To get base image
     *       use @p GetPixels() call.
     */
    LKCOMMON_INLINE const MipmapContainer& GetMipmaps() const
    {
        return mMipmaps;
    }
};

} // namespace Utils
} // namespace lkCommon

#include "ImageImpl.hpp"
