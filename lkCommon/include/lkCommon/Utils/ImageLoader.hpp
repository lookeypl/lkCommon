#pragma once

#include <lkCommon/lkCommon.hpp>

#include <string>
#include <vector>
#include <memory>


namespace lkCommon {
namespace Utils {


enum class ImageFormat: unsigned char
{
    UNKNOWN = 0,
    RGBA_UCHAR,
    RGBA_FLOAT,
};


class ImageLoader;

using ImageLoaderPtr = std::unique_ptr<ImageLoader>;

class ImageLoader
{
protected:
    // Check if file is supported on given backend. To be used by SelectLoader().
    virtual bool SupportsFile(const std::string& path) = 0;

    uint32_t mWidth;
    uint32_t mHeight;

    ImageLoader();

public:
    ~ImageLoader() = default;

    /**
     * Acquire appropriate loader for file provided in path.
     *
     * @p[in] path Path to file
     * @return Loader capable of managing the file
     */
    static ImageLoaderPtr SelectLoader(const std::string& path);

    /**
     * Parses and loads image to memory using appropriate backend.
     *
     * @p[in] path Path to file
     * @return True on success, false when loading failed.
     */
    virtual bool Load(const std::string& path) = 0;

    /**
     * Get pointer to fill with image data.
     *
     * @p[in] buf     Pointer to image's raw data. Null if image was not loaded yet.
     * @p[in] bufSize Size of buffer
     * @p[in] format  Format in which data is kept
     * @return Number of bytes filled with data. 0 if read failed or buffer has insufficient memory.
     */
    virtual size_t FillData(void* buf, const size_t bufSize, const ImageFormat format) const = 0;

    /**
     * Get image's width
     */
    LKCOMMON_INLINE uint32_t GetWidth() const
    {
        return mWidth;
    }

    /**
     * Get image's height
     */
    LKCOMMON_INLINE uint32_t GetHeight() const
    {
        return mHeight;
    }
};

} // namespace Utils
} // namespace lkCommon
