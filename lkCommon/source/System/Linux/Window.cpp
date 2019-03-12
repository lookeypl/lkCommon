#include "lkCommon/System/Window.hpp"
#include "lkCommon/lkCommon.hpp"
#include "lkCommon/Utils/Logger.hpp"

#include "Linux/XConnection.hpp"

#include <xcb/xcb_image.h>


namespace {

const char* TranslateErrorCodeToStr(int err)
{
    // these error codes base on XErrorDB, available on /usr/share/X11/XErrorDB
    switch(err)
    {
    case 0: return "OK";
    case 1: return "BadRequest";
    case 2: return "BadValue";
    case 3: return "BadWindow";
    case 4: return "BadPixmap";
    case 5: return "BadAtom";
    case 6: return "BadCursor";
    case 7: return "BadFont";
    case 8: return "BadMatch";
    case 9: return "BadDrawable";
    case 10: return "BadAccess";
    case 11: return "BadAlloc";
    case 12: return "BadColor";
    case 13: return "BadGC";
    case 14: return "BadIDChoice";
    case 15: return "BadName";
    case 16: return "BadLength";
    case 17: return "BadImplementation";
    default: return "Unknown";
    }
}

} // namespace


namespace lkCommon {
namespace System {


Window::Window() noexcept
    : mWidth(0)
    , mHeight(0)
    , mMouseDownX(0)
    , mMouseDownY(0)
    , mInitialized(false)
    , mOpened(false)
    , mInvisible(false)
    , mKeys{false}
    , mMouseButtons{false}
    , mWindow(0)
    , mDeleteReply(nullptr)
    , mGraphicsContext(0)
{
}

Window::Window(const std::string& className)
    : mWidth(0)
    , mHeight(0)
    , mMouseDownX(0)
    , mMouseDownY(0)
    , mInitialized(false)
    , mOpened(false)
    , mInvisible(false)
    , mKeys{false}
    , mMouseButtons{false}
    , mWindow(0)
    , mDeleteReply(nullptr)
    , mGraphicsContext(0)
{
    if (!Init(className))
    {
        LOGE("Failed to initialize Window");
        // TODO THROW exception
    }
}

Window::Window(const std::string& className, const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height,
               const std::string& title, const bool invisible)
    : mWidth(width)
    , mHeight(height)
    , mMouseDownX(0)
    , mMouseDownY(0)
    , mInitialized(false)
    , mOpened(false)
    , mInvisible(false)
    , mKeys{false}
    , mMouseButtons{false}
    , mWindow(0)
    , mDeleteReply(nullptr)
    , mGraphicsContext(0)
{
    if (!Init(className))
    {
        LOGE("Failed to initialize Window");
        // TODO THROW exception
    }

    SetInvisible(invisible);

    if (!Open(x, y, width, height, title))
    {
        LOGE("Failed to open Window");
        // TODO THROW exception
    }
}

Window::~Window()
{
    Close();
    Deinit();
}

bool Window::Init(const std::string& className)
{
    if (mInitialized)
    {
        LOGD("Window already initialized - ignoring call");
        return true;
    }

    if (Internal::XConnection::Instance().GetConnection() == nullptr ||
        Internal::XConnection::Instance().GetScreen() == nullptr )
    {
        LOGE("Connection with X server not initialized");
        return false;
    }

    xcb_set_screen_saver(Internal::XConnection::Instance().GetConnection(),
                         0, 0, XCB_BLANKING_NOT_PREFERRED, XCB_EXPOSURES_ALLOWED);

    mInitialized = true;

    if (!OnInit())
    {
        LOGE("Window initialization failed - OnInit callback returned an error");
        Deinit();
        return false;
    }

    return true;
}

bool Window::Open(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const std::string& title)
{
    if (mOpened)
    {
        LOGD("Window already opened - ignoring call");
        return true;
    }

    if (!mInitialized)
    {
        LOGE("Window must be initialized first");
        return false;
    }

    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    xcb_screen_t* screen = Internal::XConnection::Instance().GetScreen();
    if (connection == nullptr || screen == nullptr)
    {
        LOGE("Connection with X server not initialized");
        return false;
    }

    mWindow = xcb_generate_id(connection);

    uint32_t winValueMask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
    uint32_t winValueList[] = {
        XCB_EVENT_MASK_BUTTON_1_MOTION | XCB_EVENT_MASK_BUTTON_2_MOTION | XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_FOCUS_CHANGE |
        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        screen->default_colormap
    };

    xcb_void_cookie_t cookie = xcb_create_window_checked(connection, XCB_COPY_FROM_PARENT, mWindow,
                                       screen->root, x, y, width, height, 3, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                       screen->root_visual, winValueMask, winValueList);
    xcb_generic_error_t* err = xcb_request_check(connection, cookie);
    if (err)
    {
        LOGE("Failed to create a window: X11 protocol error " << err->error_code << " ("
             << TranslateErrorCodeToStr(err->error_code));
        free(err);
        return false;
    }

    xcb_flush(connection);

    SetTitle(title);

    xcb_intern_atom_cookie_t deleteCookie = xcb_intern_atom(connection, 1, 16, "WM_DELETE_WINDOW");
    mDeleteReply = xcb_intern_atom_reply(connection, deleteCookie, nullptr);

    xcb_intern_atom_cookie_t wmProtCookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* wmProtReply = xcb_intern_atom_reply(connection, wmProtCookie, nullptr);

    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, mWindow, wmProtReply->atom, 4, 32, 1, &mDeleteReply->atom);
    free(wmProtReply);

    if (!mInvisible)
    {
        xcb_map_window(connection, mWindow);
    }

    uint32_t gcValueMask = XCB_GC_BACKGROUND | XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t gcValue[] = { screen->white_pixel, screen->black_pixel, 0 };

    mGraphicsContext = xcb_generate_id(connection);
    cookie = xcb_create_gc(connection, mGraphicsContext, mWindow, gcValueMask, gcValue);
    err = xcb_request_check(connection, cookie);
    if (err)
    {
        LOGE("Failed to create graphics context: X11 protocol error " << err->error_code << " ("
            << TranslateErrorCodeToStr(err->error_code));
        free(err);
        return false;
    }

    xcb_flush(connection);

    mOpened = true;
    mWidth = width;
    mHeight = height;

    if (!OnOpen())
    {
        LOGE("Window initialization failed - OnOpen callback returned an error");
        Close();
        return false;
    }

    return true;
}

bool Window::SetTitle(const std::string& title)
{
    if (mOpened)
    {
        xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
        if (connection == nullptr)
        {
            LOGE("Connection with X server not initialized");
            return false;
        }

        xcb_change_property(connection, XCB_PROP_MODE_REPLACE, mWindow, XCB_ATOM_WM_NAME,
                            XCB_ATOM_STRING, 8, title.size(), title.c_str());
    }

    return true;
}

void Window::SetInvisible(bool invisible)
{
    mInvisible = invisible;

    if (mOpened && mWindow)
    {
        xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
        if (connection == nullptr)
        {
            LOGE("Connection with X server not initialized");
            return;
        }

        if (mInvisible)
            xcb_unmap_window(connection, mWindow);
        else
            xcb_map_window(connection, mWindow);
    }
}

bool Window::DisplayImage(uint32_t x, uint32_t y, const WindowImage& image)
{
    if ((x + image.GetWidth() > mWidth) || (y + image.GetHeight() > mHeight))
    {
        LOGE("Displayed Image extens beyond Window borders");
        return false;
    }

    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    if (connection == nullptr)
    {
        LOGE("Connection with X server not initialized");
        return false;
    }

    xcb_image_t* img = reinterpret_cast<xcb_image_t*>(image.GetHandle());
    xcb_void_cookie_t c = xcb_image_put(connection, mWindow, mGraphicsContext, img, x, y, 0);
    xcb_generic_error_t* err = xcb_request_check(connection, c);
    if (err)
    {
        LOGE("Failed to put image on window: X11 protocol error " << err->error_code << " ("
            << TranslateErrorCodeToStr(err->error_code));
        free(err);
        return false;
    }

    xcb_flush(connection);

    return true;
}

void Window::MouseButtonDown(const uint32_t button, const uint32_t x, const uint32_t y)
{
    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    if (connection == nullptr)
    {
        LOGE("Connection with X server not initialized");
        return;
    }

    xcb_grab_pointer_cookie_t grab = xcb_grab_pointer(connection, 1, mWindow, 0, XCB_GRAB_MODE_ASYNC,
                                                      XCB_GRAB_MODE_ASYNC, mWindow, XCB_NONE, XCB_CURRENT_TIME);
    xcb_grab_pointer_reply_t* grabReply = xcb_grab_pointer_reply(connection, grab, nullptr);
    free(grabReply);

    mMouseButtons[button] = true;
    mMouseDownX = x;
    mMouseDownY = y;
    OnMouseDown(button);
}

void Window::MouseButtonUp(const uint32_t button)
{
    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    if (connection == nullptr)
    {
        LOGE("Connection with X server not initialized");
        return;
    }
    xcb_ungrab_pointer(connection, XCB_CURRENT_TIME);

    mMouseButtons[button] = false;
    OnMouseUp(button);
}

void Window::MouseMove(const uint32_t x, const uint32_t y)
{
    OnMouseMove(x, y,
                static_cast<int32_t>(x) - static_cast<int32_t>(mMouseDownX),
                static_cast<int32_t>(y) - static_cast<int32_t>(mMouseDownY));
    mMouseDownX = x;
    mMouseDownY = y;
}

void Window::ProcessMessages()
{
    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    if (connection == nullptr)
    {
        LOGE("Connection with X server not initialized");
        return;
    }

    xcb_flush(connection);

    xcb_generic_event_t* event;
    while ((event = xcb_poll_for_event(connection)))
    {
        switch (event->response_type & ~0x80)
        {
        case XCB_CLIENT_MESSAGE:
        {
            xcb_client_message_event_t* cm = reinterpret_cast<xcb_client_message_event_t*>(event);

            if (static_cast<xcb_atom_t>(cm->data.data32[0]) == mDeleteReply->atom)
                this->Close();

            break;
        }
        case XCB_KEY_PRESS:
        {
            xcb_key_press_event_t* kp = reinterpret_cast<xcb_key_press_event_t*>(event);
            mKeys[kp->detail] = true;
            OnKeyDown(static_cast<KeyCode>(kp->detail));
            break;
        }
        case XCB_KEY_RELEASE:
        {
            xcb_key_release_event_t* kr = reinterpret_cast<xcb_key_release_event_t*>(event);
            mKeys[kr->detail] = false;
            OnKeyUp(static_cast<KeyCode>(kr->detail));
            break;
        }
        case XCB_MOTION_NOTIFY:
        {
            xcb_motion_notify_event_t* m = reinterpret_cast<xcb_motion_notify_event_t*>(event);
            MouseMove(m->event_x, m->event_y);
            break;
        }
        case XCB_BUTTON_PRESS:
        {
            xcb_button_press_event_t* bp = reinterpret_cast<xcb_button_press_event_t*>(event);

            if (bp->detail < 4) // 1-3 MBtns, 4-5 MWheel
                MouseButtonDown(bp->detail - 1, bp->event_x, bp->event_y);

            break;
        }
        case XCB_BUTTON_RELEASE:
        {
            xcb_button_release_event_t* br = reinterpret_cast<xcb_button_release_event_t*>(event);
            MouseButtonUp(br->detail - 1);
            break;
        }
        case XCB_CONFIGURE_NOTIFY:
        {
            xcb_configure_notify_event_t* cne = reinterpret_cast<xcb_configure_notify_event_t*>(event);

            if (cne->width != mWidth || cne->height != mHeight)
            {
                mWidth = cne->width;
                mHeight = cne->height;
                OnResize(mWidth, mHeight);
            }

            break;
        }
        }

        free(event);
    }
}

void Window::Update(float deltaTime)
{
    ProcessMessages();
    OnUpdate(deltaTime);
}

void Window::Close()
{
    if (!mOpened)
    {
        LOGD("Window is not opened - ignoring call");
        return;
    }

    OnClose();

    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    if (connection == nullptr)
    {
        LOGE("Connection with X server not initialized");
        return;
    }

    if (!mInvisible)
        xcb_unmap_window(connection, mWindow);

    if (mDeleteReply)
    {
        free(mDeleteReply);
        mDeleteReply = nullptr;
    }

    if (mGraphicsContext)
    {
        xcb_free_gc(connection, mGraphicsContext);
        mGraphicsContext = 0;
    }

    if (mWindow)
    {
        xcb_destroy_window(connection, mWindow);
        mWindow = 0;
    }

    mOpened = false;
}

void Window::Deinit()
{
    if (!mInitialized)
    {
        LOGD("Window is not initialized - ignoring call");
        return;
    }

    if (mOpened)
    {
        LOGW("Deinit() called while Window still opened - closing");
        Close();
    }

    OnDeinit();

    xcb_connection_t* connection = Internal::XConnection::Instance().GetConnection();
    if (connection != nullptr)
    {
        xcb_set_screen_saver(connection, -1, 0, XCB_BLANKING_NOT_PREFERRED, XCB_EXPOSURES_ALLOWED);
    }
}


// callbacks

bool Window::OnInit()
{
    return true;
}

bool Window::OnOpen()
{
    return true;
}

void Window::OnResize(const uint32_t width, const uint32_t height)
{
    LKCOMMON_UNUSED(width);
    LKCOMMON_UNUSED(height);
}

void Window::OnClose()
{
}

void Window::OnDeinit()
{
}

void Window::OnKeyDown(const KeyCode key)
{
    LKCOMMON_UNUSED(key);
}

void Window::OnKeyUp(const KeyCode key)
{
    LKCOMMON_UNUSED(key);
}

void Window::OnUpdate(const float deltaTime)
{
    LKCOMMON_UNUSED(deltaTime);
}

void Window::OnMouseDown(const uint32_t key)
{
    LKCOMMON_UNUSED(key);
}

void Window::OnMouseMove(const uint32_t x, const uint32_t y, const int32_t deltaX, const int32_t deltaY)
{
    LKCOMMON_UNUSED(x);
    LKCOMMON_UNUSED(y);
    LKCOMMON_UNUSED(deltaX);
    LKCOMMON_UNUSED(deltaY);
}

void Window::OnMouseUp(const uint32_t key)
{
    LKCOMMON_UNUSED(key);
}

} // namespace System
} // namespace lkCommon
