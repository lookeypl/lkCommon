/**
 * @file
 * @author LKostyra (costyrra.xl@gmail.com)
 * @brief  Image module implementation
 */

#pragma once

#ifndef _LKCOMMON_UTILS_IMAGE_HPP_
#error "Please include main header of Image, not the implementation header."
#endif // _LKCOMMON_UTILS_IMAGE_HPP_

#include "lkCommon/Utils/Logger.hpp"
#include "lkCommon/Math/Utilities.hpp"


namespace lkCommon {
namespace Utils {

template <typename PixelType>
Image<PixelType>::Image()
    : mWidth(0)
    , mHeight(0)
    , mWidthStep(0)
    , mHeightStep(0)
    , mPixels()
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
}

template <typename PixelType>
Image<PixelType>::Image(uint32_t width, uint32_t height)
    : mWidth(width)
    , mHeight(height)
    , mWidthStep(1.0f / static_cast<float>(width))
    , mHeightStep(1.0f / static_cast<float>(height))
    , mPixels(mWidth * mHeight)
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
}

template <typename PixelType>
Image<PixelType>::Image(uint32_t width, uint32_t height, uint32_t pixelsPerRow, const Image<PixelType>::PixelContainer& data, bool isBGR)
    : mWidth(width)
    , mHeight(height)
    , mWidthStep(1.0f / static_cast<float>(width))
    , mHeightStep(1.0f / static_cast<float>(height))
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
    , mWidthStep(other.mWidthStep)
    , mHeightStep(other.mHeightStep)
    , mPixels(other.mPixels)
    , mWindowImage(mWidth, mHeight, mPixels.data())
{
}

template <typename PixelType>
Image<PixelType>::Image(Image<PixelType>&& other)
    : mWidth(std::move(other.mWidth))
    , mHeight(std::move(other.mHeight))
    , mWidthStep(std::move(other.mWidthStep))
    , mHeightStep(std::move(other.mHeightStep))
    , mPixels(std::move(other.mPixels))
    , mWindowImage(std::move(other.mWindowImage))
{
}

template <typename PixelType>
Image<PixelType>& Image<PixelType>::operator=(const Image<PixelType>& other)
{
    mWidth = other.mWidth;
    mHeight = other.mHeight;
    mWidthStep = other.mWidthStep;
    mHeightStep = other.mHeightStep;
    mPixels = other.mPixels;
    mWindowImage.Recreate(mWidth, mHeight, mPixels.data());
}

template <typename PixelType>
Image<PixelType>& Image<PixelType>::operator=(Image<PixelType>&& other)
{
    mWidth = std::move(other.mWidth);
    mHeight = std::move(other.mHeight);
    mWidthStep = std::move(other.mWidthStep);
    mHeightStep = std::move(other.mHeightStep);
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
size_t Image<PixelType>::GetPixelCoordWrapped(uint32_t x, uint32_t y)
{
    if (x >= mWidth)
    {
        x -= (mWidth * (x / mWidth));
    }
    if (y >= mHeight)
    {
        y -= (mHeight * (y / mHeight));
    }

    return y * mWidth + x;
}

template <typename PixelType>
PixelType Image<PixelType>::SampleNearest(float x, float y)
{
    PixelType ret = mPixels[GetPixelCoordWrapped(
        static_cast<uint32_t>(x * mWidth),
        static_cast<uint32_t>(y * mHeight)
    )];

    ret.Swap(0, 2);
    return ret;
}

template <typename PixelType>
PixelType Image<PixelType>::SampleBilinear(float x, float y)
{
    const float xCoord = x * mWidth;
    const float yCoord = y * mHeight;
    const int xIntCoord = static_cast<int>(xCoord);
    const int yIntCoord = static_cast<int>(yCoord);
    const float xDecCoord = xCoord - xIntCoord;
    const float yDecCoord = yCoord - yIntCoord;

    const size_t coords[] {
        GetPixelCoordWrapped(xIntCoord    , yIntCoord),
        GetPixelCoordWrapped(xIntCoord + 1, yIntCoord),
        GetPixelCoordWrapped(xIntCoord    , yIntCoord + 1),
        GetPixelCoordWrapped(xIntCoord + 1, yIntCoord + 1)
    };

    const PixelType R1 = lkCommon::Math::Util::Lerp(mPixels[coords[0]], mPixels[coords[1]], xDecCoord);
    const PixelType R2 = lkCommon::Math::Util::Lerp(mPixels[coords[2]], mPixels[coords[3]], xDecCoord);
    PixelType R = lkCommon::Math::Util::Lerp(R1, R2, yDecCoord);

    R.Swap(0, 2);
    return R;
}

template <typename PixelType>
bool Image<PixelType>::Resize(uint32_t width, uint32_t height)
{
    if (mWidth == width && mHeight == height)
        return true;

    if (width == 0 || height == 0)
    {
        LOGE("Invalid parameters - provided width or height equals to zero");
        return false;
    }

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
PixelType Image<PixelType>::Sample(float x, float y, Sampling samplingType)
{
    // skip sampling if we have 1x1 dimensions
    if (mWidth == 1 && mHeight == 1)
        return mPixels[0];

    switch (samplingType)
    {
    case Sampling::NEAREST: return SampleNearest(x, y);
    case Sampling::BILINEAR: return SampleBilinear(x, y);
    default: return PixelType();
    }
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
