#include "PCH.hpp"
#include "../Window.hpp"
#include "../lkCommon.hpp"
#include "../Logger.hpp"


namespace lkCommon {

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


Window::Window()
    : mConnection(nullptr)
    , mWindow(0)
    , mScreen(nullptr)
    , mDeleteReply(nullptr)
    , mConnScreen(0)
    , mWidth(0)
    , mHeight(0)
    , mMouseDownX(0)
    , mMouseDownY(0)
    , mOpened(false)
    , mInvisible(false)
    , mKeys{false}
    , mMouseButtons{false}
{
}

Window::~Window()
{
    Close();

    if (mDeleteReply)
        free(mDeleteReply);

    if (mConnection)
    {
        xcb_set_screen_saver(mConnection, -1, 0, XCB_BLANKING_NOT_PREFERRED, XCB_EXPOSURES_ALLOWED);
        if (mWindow)
            xcb_destroy_window(mConnection, mWindow);
        xcb_flush(mConnection);
        xcb_disconnect(mConnection);
    }
}

bool Window::Init()
{
    mConnection = xcb_connect(nullptr, &mConnScreen);
    if (xcb_connection_has_error(mConnection))
    {
        LOGE("Failed to connect to X server through XCB");
        return false;
    }

    const xcb_setup_t* xcbSetup;
    xcb_screen_iterator_t xcbScreenIt;

    xcbSetup = xcb_get_setup(mConnection);
    xcbScreenIt = xcb_setup_roots_iterator(xcbSetup);
    while (mConnScreen-- > 0)
        xcb_screen_next(&xcbScreenIt);
    mScreen = xcbScreenIt.data;

    xcb_set_screen_saver(mConnection, 0, 0, XCB_BLANKING_NOT_PREFERRED, XCB_EXPOSURES_ALLOWED);

    OnInit();
    return true;
}

bool Window::Open(int x, int y, int width, int height, const std::string& title)
{
    mWidth = width;
    mHeight = height;

    if (mConnection == nullptr)
    {
        LOGE("Window not initialized - cannot open");
        return false;
    }

    mWindow = xcb_generate_id(mConnection);

    xcb_colormap_t colormap = xcb_generate_id(mConnection);
    xcb_create_colormap(mConnection, XCB_COLORMAP_ALLOC_NONE, colormap, mScreen->root, mScreen->root_visual);

    uint32_t valueMask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
    uint32_t valueList[] = {
        XCB_EVENT_MASK_BUTTON_1_MOTION | XCB_EVENT_MASK_BUTTON_2_MOTION | XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_FOCUS_CHANGE |
        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        colormap
    };

    xcb_void_cookie_t cookie = xcb_create_window_checked(mConnection, XCB_COPY_FROM_PARENT, mWindow,
                                                         mScreen->root, x, y, mWidth, mHeight, 3,
                                                         XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                                         mScreen->root_visual, valueMask, valueList);

    xcb_generic_error_t* err = xcb_request_check(mConnection, cookie);
    if (err)
    {
        LOGE("Failed to create a window: X11 protocol error " << err->error_code << " ("
             << TranslateErrorCodeToStr(err->error_code));
        free(err);
        return false;
    }

    SetTitle(title);

    xcb_intern_atom_cookie_t deleteCookie = xcb_intern_atom(mConnection, 1, 16, "WM_DELETE_WINDOW");
    mDeleteReply = xcb_intern_atom_reply(mConnection, deleteCookie, nullptr);

    xcb_intern_atom_cookie_t wmProtCookie = xcb_intern_atom(mConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* wmProtReply = xcb_intern_atom_reply(mConnection, wmProtCookie, nullptr);

    xcb_change_property(mConnection, XCB_PROP_MODE_REPLACE, mWindow, wmProtReply->atom, 4, 32, 1, &mDeleteReply->atom);
    free(wmProtReply);

    if (!mInvisible)
        xcb_map_window(mConnection, mWindow);

    OnOpen();
    mOpened = true;
    return true;
}

bool Window::SetTitle(const std::wstring& title)
{
    LKCOMMON_UNUSED(title);
    LOGE("Setting a widechar title on Linux is unsupported");
    return false;
}

bool Window::SetTitle(const std::string& title)
{
    if (mOpened)
    {
        xcb_change_property(mConnection, XCB_PROP_MODE_REPLACE, mWindow, XCB_ATOM_WM_NAME,
                            XCB_ATOM_STRING, 8, title.size(), title.c_str());
    }

    return true;
}

void Window::SetInvisible(bool invisible)
{
    mInvisible = invisible;

    if (mOpened && mWindow)
    {
        if (mInvisible)
            xcb_unmap_window(mConnection, mWindow);
        else
            xcb_map_window(mConnection, mWindow);
    }
}

void Window::MouseButtonDown(int button, int x, int y)
{
    xcb_grab_pointer_cookie_t grab = xcb_grab_pointer(mConnection, 1, mWindow, 0, XCB_GRAB_MODE_ASYNC,
                                                      XCB_GRAB_MODE_ASYNC, mWindow, XCB_NONE, XCB_CURRENT_TIME);
    xcb_grab_pointer_reply_t* grabReply = xcb_grab_pointer_reply(mConnection, grab, nullptr);
    free(grabReply);

    mMouseButtons[button] = true;
    mMouseDownX = x;
    mMouseDownY = y;
    OnMouseDown(button);
}

void Window::MouseButtonUp(int button)
{
    xcb_ungrab_pointer(mConnection, XCB_CURRENT_TIME);

    mMouseButtons[button] = false;
    OnMouseUp(button);
}

void Window::MouseMove(int x, int y)
{
    OnMouseMove(x, y, x - mMouseDownX, y - mMouseDownY);
    mMouseDownX = x;
    mMouseDownY = y;
}

void Window::ProcessMessages()
{
    xcb_flush(mConnection);

    xcb_generic_event_t* event;
    while ((event = xcb_poll_for_event(mConnection)))
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
            // TODO resize support goes here
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
        return;

    OnClose();
    mOpened = false;
}


// callbacks

void Window::OnInit()
{
}

void Window::OnOpen()
{
}

void Window::OnClose()
{
}

void Window::OnKeyDown(KeyCode key)
{
    LKCOMMON_UNUSED(key);
}

void Window::OnKeyUp(KeyCode key)
{
    LKCOMMON_UNUSED(key);
}

void Window::OnUpdate(float deltaTime)
{
    LKCOMMON_UNUSED(deltaTime);
}

void Window::OnMouseDown(int key)
{
    LKCOMMON_UNUSED(key);
}

void Window::OnMouseMove(int x, int y, int deltaX, int deltaY)
{
    LKCOMMON_UNUSED(x);
    LKCOMMON_UNUSED(y);
    LKCOMMON_UNUSED(deltaX);
    LKCOMMON_UNUSED(deltaY);
}

void Window::OnMouseUp(int key)
{
    LKCOMMON_UNUSED(key);
}

} // namespace lkCommon
