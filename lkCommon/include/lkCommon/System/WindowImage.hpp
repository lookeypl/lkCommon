#pragma once

#if defined(__linux__) | defined(__LINUX__)
#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#elif !defined(WIN32)
#error "Platform not supported"
#endif


namespace lkCommon {
namespace System {

/**
 * Class unifying various OS-specific Image objects into one entity.
 */
class WindowImage final
{
    uint32_t mWidth;
    uint32_t mHeight;
#ifdef WIN32
    // All Window::DisplayImage() cares about on Windows is a raw data pointer
    void* mDataPtr;
#elif defined(__linux__) | defined(__LINUX__)
    xcb_image_t* mXcbImage;
#else
#error "Platform not supported"
#endif

public:
    /**
     * Creates uninitialized window image. Such object must be initialized with
     * Recreate() before use.
     */
    WindowImage();

    /**
     * Creates window image.
     */
    WindowImage(uint32_t width, uint32_t height, void* data);

    /**
     * Destroys window image.
     */
    ~WindowImage();

    WindowImage(const WindowImage& other) = delete;
    WindowImage& operator=(const WindowImage& other) = delete;
    WindowImage(WindowImage&& other) = default;
    WindowImage& operator=(WindowImage&& other) = default;

    /**
     * Recreates window image to have new size.
     */
    bool Recreate(uint32_t width, uint32_t height, void* data);

    /**
     * Acquires platform-specific bitmap handle.
     *
     * @note This function should be used ONLY by platform-specific modules.
     * Result depends on used platform:
     *   - void* data pointer on Windows
     *   - xcb_image* on Linux
     */
    void* GetHandle() const;

    LKCOMMON_INLINE uint32_t GetWidth() const
    {
        return mWidth;
    }

    LKCOMMON_INLINE uint32_t GetHeight() const
    {
        return mHeight;
    }
};

} // namespace System
} // namespace lkCommon
