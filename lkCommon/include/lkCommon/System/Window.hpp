#pragma once

#include "lkCommon/lkCommon.hpp"
#include "lkCommon/System/KeyCodes.hpp"
#include "lkCommon/System/WindowImage.hpp"

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#elif defined(__linux__) || defined(__LINUX__)
#include <xcb/xcb.h>
#endif


namespace lkCommon {
namespace System {

/**
 * Class used to create a predefined Window in host OS.
 */
class Window
{
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mMouseDownX;
    uint32_t mMouseDownY;
    bool mInitialized;
    bool mOpened;
    bool mInvisible;
    bool mKeys[255];
    bool mMouseButtons[3];

#ifdef WIN32
    HINSTANCE mInstance;
    HWND mHWND;
    HDC mHDC;
    std::wstring mClassName;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#elif defined(__linux__) || defined(__LINUX__)
    xcb_window_t mWindow;
    xcb_intern_atom_reply_t* mDeleteReply;
    xcb_gcontext_t mGraphicsContext; // used for setting single pixels on screen

    void ProcessMessages();
#else
#error "Target platform not supported"
#endif

    void MouseButtonDown(const uint32_t button, const uint32_t x, const uint32_t y);
    void MouseButtonUp(const uint32_t button);
    void MouseMove(const uint32_t x, const uint32_t y);

protected:
    /**
     * Allows to check whether given keyboard key is pressed.
     *
     * @p[in] key   Checks if given key was pressed
     * @return True if key @p key was pressed, false otherwise.
     */
    LKCOMMON_INLINE bool IsKeyPressed(const KeyCode key) const
    {
        return mKeys[static_cast<std::underlying_type<KeyCode>::type>(key)];
    }

    /**
     * Allows to quickly check whether given mouse key is pressed.
     *
     * @p[in] key   Checks if given key was pressed
     * @return True if key @p key was pressed, false otherwise.
     */
    LKCOMMON_INLINE bool IsMouseKeyPressed(const int key) const
    {
        return mMouseButtons[key];
    }

    /**
     * @defgroup WindowCallbacks Callbacks, which can be defined by inheriting Window class. Called at different
     *                           events and function calls related to Window.
     * @{
     */

    /**
     * Callback called at the end of Init() function call. Can fail, which will revert Init() functions
     * effects and return an error.
     *
     * @return Set to true to mark user's part of initialization as successful. Set to false to mark an error.
     */
    virtual bool OnInit();

    /**
     * Callback called at the end of Open() function call. Can fail, which will revert Open() function's
     * effects and return an error.
     *
     * @return Set to true to mark user's part of window opening as successful. Set to false to mark an error.
     */
    virtual bool OnOpen();

    /**
     * Callback called whenever Window receives Resize event.
     *
     * @p[in] width     New width of Window.
     * @p[in] height    New height of Window.
     */
    virtual void OnResize(const uint32_t width, const uint32_t height);

    /**
     * Callback called at the end of Close() function call.
     */
    virtual void OnClose();

    /**
     * Callback called whenever Window receives key down event.
     *
     * @p[in] key   Code of key which was pressed down.
     */
    virtual void OnKeyDown(const KeyCode key);

    /**
     * Callback called whenever Window receives key up event.
     *
     * @p[in] key   Code of key which was depressed.
     */
    virtual void OnKeyUp(const KeyCode key);

    /**
     * Callback called whenever Window's Update() function is called.
     *
     * @p[in] deltaTime Time provided by user on Update() call. Window class does not track times between Update() calls.
     */
    virtual void OnUpdate(const float deltaTime);

    /**
     * Callback called whenever a mouse button is pressed down.
     *
     * @p[in] key   Value of pressed mouse button.
     */
    virtual void OnMouseDown(const uint32_t key);

    /**
     * Callback called whenever a mouse button is let go.
     *
     * @p[in] key   Value of depressed mouse button.
     */
    virtual void OnMouseUp(const uint32_t key);

    /**
     * Callback called whenever Window receives a mouse move event.
     *
     * @p[in] x         Current X coordinate of mouse cursor.
     * @p[in] y         Current Y coordinate of mouse cursor.
     * @p[in] deltaX    Difference between current and previous mouse position, in X coordinate.
     * @p[in] deltaY    Difference between current and previous mouse position, in Y coordinate.
     */
    virtual void OnMouseMove(const uint32_t x, const uint32_t y, const int32_t deltaX, const int32_t deltaY);

    /**
     * @}
     */

public:
    /**
     * Window class default constructor. After construction window is NOT initialized, or opened.
     */
    Window() noexcept;

    /**
     * Window class constructor. Calls Init(), providing a class name and connecting to host OS.
     *
     * @p[in] className Window's class name used to register the window (if needed on some host OS's). Can be left empty.
     */
    Window(const std::string& className);

    /**
     * Window class constructor. Initializes and opens a window.
     *
     * @p[in] className Window's class name used to register the window (if needed on some host OS's). Can be left empty.
     * @p[in] x         Window's initial position, X coordinate.
     * @p[in] y         Window's initial position, Y coordinate.
     * @p[in] width     Window's initial width.
     * @p[in] height    Window's initial height.
     * @p[in] title     Window's title.
     * @p[in] invisible Set to true if Window should be created invisible. Defaults to false.
     */
    Window(const std::string& className, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height,
           const std::string& title, const bool invisible = false);

    /**
     * Window class destructor. Will call Close() and Deinit().
     */
    ~Window();

    /**
     * Initializes a Window. This function will connect to host OS and prepare the class for further use.
     *
     * @p[in] className Window's class name used to register the window (if needed on some host OS's). Can be omitted.
     * @return True if initialization succeeds, false in case of an error.
     *
     * @note If window was already initialized, function does nothing and succeeds.
     */
    bool Init(const std::string& className = std::string());

    /**
     * Opens new window with given parameters.
     *
     * @p[in] x         Window's initial position, X coordinate.
     * @p[in] y         Window's initial position, Y coordinate.
     * @p[in] width     Window's initial width.
     * @p[in] height    Window's initial height.
     * @p[in] title     Window's title.
     * @return True if opening a window succeeded, false otherwise.
     *
     * @note If window was already opened, function does nothing and succeeds.
     */
    bool Open(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& title);

    /**
     * Sets window's title bar contents to new value.
     *
     * @p[in] title New title to be displayed in Window's top bar.
     * @return True if succeeded, false otherwise.
     */
    bool SetTitle(const std::string& title);

    /**
     * Sets whether the window should be invisible. Already opened window will disappear (or appear).
     *
     * @p[in] invisible Set to true if window should be invisible.
     */
    void SetInvisible(const bool invisible);

    /**
     * Displays an user-defined image inside Window's contents.
     *
     * @p[in] x     X coordinate where an Image should be displayed, from top-left corner.
     * @p[in] y     Y coordinate where an Image should be displayed, from top-left corner.
     * @p[in] image Image to be displayed.
     *
     * @note WindowImage object can be acquired from Image template class. See Image class for more info.
     * @see lkCommon::Utils::Image
     */
    bool DisplayImage(const uint32_t x, const uint32_t y, const WindowImage& image);

    /**
     * Process update loop of Window.
     *
     * @p[in] deltaTime Time which will be passed to OnUpdate() callback.
     *
     * TODO Allow providing custom payloads for Update call
     * TODO Remove deltaTime and calculate it manually
     */
    void Update(const float deltaTime);

    /**
     * Closes currently opened Window.
     */
    void Close();

    /**
     * Deinitializes Window class, freeing any connections to host OS.
     */
    void Deinit();

    /**
     * Get current window's width
     *
     * @return Window's width
     */
    LKCOMMON_INLINE uint32_t GetWidth() const
    {
        return mWidth;
    }

    /**
     * Get current window's height
     *
     * @return Window's height
     */
    LKCOMMON_INLINE uint32_t GetHeight() const
    {
        return mHeight;
    }

    /**
     * Check if window is initialized (state after calling Init() but before calling Deinit()).
     *
     * @return True if window was initialized, false otherwise.
     */
    LKCOMMON_INLINE bool IsInitialized() const
    {
        return mInitialized;
    }

    /**
     * Check if window is opened (state after calling Open() but before calling Close()).
     *
     * @return True if window was opened, false otherwise.
     */
    LKCOMMON_INLINE bool IsOpened() const
    {
        return mOpened;
    }

    /**
     * Check if window is invisible.
     *
     * @return True if invisible, false otherwise
     */
    LKCOMMON_INLINE bool IsInvisible() const
    {
        return mInvisible;
    }

#ifdef WIN32
    /**
     * Returns Windows API Instance object used for operations.
     *
     * @return Windows API Instance handle, NULL if Init() was not yet called (or failed).
     */
    LKCOMMON_INLINE HINSTANCE GetInstance() const
    {
        return mInstance;
    }

    /**
     * Returns Windows API handle to created Window.
     *
     * @return Windows API handle to window, INVALID_HANDLE_VALUE if Open() was not yet called (or failed).
     */
    LKCOMMON_INLINE HWND GetHandle() const
    {
        return mHWND;
    }
#elif defined(__linux__) || defined(__LINUX__)
    /**
     * Get X Window pointer.
     *
     * @return X Window pointer, nullptr if Open() was not called yet.
     */
    LKCOMMON_INLINE xcb_window_t GetWindow() const
    {
        return mWindow;
    }
#else
#error "Target platform not supported"
#endif
};

} // namespace System
} // namespace lkCommon
