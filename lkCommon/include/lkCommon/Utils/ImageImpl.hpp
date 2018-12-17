/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Image module implementation
 */

#pragma once

#include "lkCommon/Utils/Logger.hpp"


namespace lkCommon {
namespace Utils {

template <typename PixelType>
Image<PixelType>::Image()
    : mWidth(0)
    , mHeight(0)
    , mPixels()
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
}

template <typename PixelType>
Image<PixelType>::Image(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
    , mPixels(mWidth * mHeight)
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
}

template <typename PixelType>
Image<PixelType>::Image(uint32_t width, uint32_t height, uint32_t pixelsPerRow, const Image<PixelType>::PixelContainer& data, bool isBGR)
    : mWidth(width)
    , mHeight(height)
    , mPixels(mWidth * mHeight)
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
    if (pixelsPerRow == 0)
    {
        pixelsPerRow = width;
    }

    size_t srcRows = data.size() / pixelsPerRow;
    size_t rowsToCopy = srcRows < height ? srcRows : height;
    size_t pixelsToCopy = pixelsPerRow < width ? pixelsPerRow : width;
    size_t srcIndex = 0;
    size_t dstIndex = 0;
    for (size_t y = 0; y < rowsToCopy; ++y)
    {
        srcIndex = y * pixelsPerRow;
        dstIndex = y * width;

        for (size_t x = 0; x < pixelsToCopy; ++x)
        {
            mPixels[dstIndex] = data[srcIndex];
            if (!isBGR)
                mPixels[dstIndex].Swap(0, 2);

            ++srcIndex;
            ++dstIndex;
        }
    }
}

template <typename PixelType>
Image<PixelType>::Image(const Image<PixelType>& other)
    : mWidth(other.mWidth)
    , mHeight(other.mHeight)
    , mPixels(other.mPixels)
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
}

template <typename PixelType>
Image<PixelType>::Image(Image<PixelType>&& other)
    : mWidth(std::move(other.mWidth))
    , mHeight(std::move(other.mHeight))
    , mPixels(std::move(other.mPixels))
    , mWindowImage(std::move(other.mWindowImage))
{
}

template <typename PixelType>
Image<PixelType>& Image<PixelType>::operator=(const Image<PixelType>& other)
{
    mWidth = other.mWidth;
    mHeight = other.mHeight;
    mPixels = other.mPixels;
    mWindowImage.Recreate(mWidth, mHeight, mPixels.data());
}

template <typename PixelType>
Image<PixelType>& Image<PixelType>::operator=(Image<PixelType>&& other)
{
    mWidth = std::move(other.mWidth);
    mHeight = std::move(other.mHeight);
    mPixels = std::move(other.mPixels);
    mWindowImage = std::move(other.mWindowImage);
}



template <typename PixelType>
Image<PixelType>::~Image()
{
}

template <typename PixelType>
size_t Image<PixelType>::GetPixelCoord(uint32_t x, uint32_t y)
{
    if (x >= mWidth || y >= mHeight)
    {
        LOGE("Requested pixel coordinates (" << x << ", " << y << ") extend too far - " <<
             "limits (" << mWidth << ", " << mHeight << ") shouldn't be met, or crossed.");
        return SIZE_MAX;
    }

    return y * mWidth + x;
}

template <typename PixelType>
bool Image<PixelType>::Resize(uint32_t width, uint32_t height)
{
    mWidth = width;
    mHeight = height;

    try {
        mPixels.resize(mWidth * mHeight);
    } catch (std::exception& e) {
        LOGE("Failed to resize Image: " << e.what());
        return false;
    }

    return mWindowImage.Recreate(width, height, mPixels.data());
}

template <typename PixelType>
bool Image<PixelType>::SetPixel(uint32_t x, uint32_t y, const PixelType& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        return false;
    }

    mPixels[coord] = pixel;
    mPixels[coord].Swap(0, 2);
    return true;
}

template <typename PixelType>
bool Image<PixelType>::GetPixel(uint32_t x, uint32_t y, PixelType& pixel)
{
    size_t coord = GetPixelCoord(x, y);
    if (coord == SIZE_MAX)
    {
        return false;
    }

    pixel = mPixels[coord];
    pixel.Swap(0, 2);
    return true;
}

template <typename PixelType>
void Image<PixelType>::SetAllPixels(const PixelType& color)
{
    for (uint32_t i = 0; i < mPixels.size(); ++i)
        mPixels[i] = color;
}

template <typename PixelType>
template <typename ConvType>
Image<PixelType>::operator Image<ConvType>() const
{
    typename Image<ConvType>::PixelContainer resultData(mPixels.size());

    for (uint32_t i = 0; i < mPixels.size(); ++i)
        resultData[i] = static_cast<ConvType>(mPixels[i]);

    return Image<ConvType>(mWidth, mHeight, 0, resultData, true);
}

} // namespace Utils
} // namespace lkCommon
