#include "lkCommon/System/Window.hpp"
#include "lkCommon/Utils/Logger.hpp"
#include "lkCommon/Utils/StringConv.hpp"


namespace lkCommon {
namespace System {

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
const DWORD WINDOWED_EX_STYLE = WS_EX_WINDOWEDGE;
const DWORD WINDOWED_STYLE = WS_OVERLAPPEDWINDOW;

Window::Window() noexcept
    : mWidth(0)
    , mHeight(0)
    , mMouseDownX(0)
    , mMouseDownY(0)
    , mInitialized(false)
    , mOpened(false)
    , mInvisible(false)
    , mKeys{false, }
    , mMouseButtons{false, }
    , mInstance(NULL)
    , mHWND(static_cast<HWND>(INVALID_HANDLE_VALUE))
    , mHDC(static_cast<HDC>(INVALID_HANDLE_VALUE))
    , mClassName()
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
    , mKeys{ false, }
    , mMouseButtons{ false, }
    , mInstance(NULL)
    , mHWND(static_cast<HWND>(INVALID_HANDLE_VALUE))
    , mHDC(static_cast<HDC>(INVALID_HANDLE_VALUE))
    , mClassName()
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
    , mKeys{ false, }
    , mMouseButtons{false, }
    , mInstance(NULL)
    , mHWND(static_cast<HWND>(INVALID_HANDLE_VALUE))
    , mHDC(static_cast<HDC>(INVALID_HANDLE_VALUE))
    , mClassName()
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

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (wnd == nullptr)
        return DefWindowProc(hWnd, msg, wParam, lParam);

    switch (msg)
    {
    case WM_CLOSE:
        wnd->Close();
        break;

    case WM_SIZE:
        wnd->mWidth = LOWORD(lParam);
        wnd->mHeight = HIWORD(lParam);
        wnd->OnResize(wnd->mWidth, wnd->mHeight);
        break;

    case WM_KEYDOWN:
        wnd->mKeys[wParam] = true;
        wnd->OnKeyDown(static_cast<KeyCode>(wParam));
        break;

    case WM_KEYUP:
        wnd->mKeys[wParam] = false;
        wnd->OnKeyUp(static_cast<KeyCode>(wParam));
        break;

    case WM_LBUTTONDOWN:
        wnd->MouseButtonDown(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_LBUTTONUP:
        wnd->MouseButtonUp(0);
        break;

    case WM_RBUTTONDOWN:
        wnd->MouseButtonDown(1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_RBUTTONUP:
        wnd->MouseButtonUp(1);
        break;

    case WM_MBUTTONDOWN:
        wnd->MouseButtonDown(2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    case WM_MBUTTONUP:
        wnd->MouseButtonUp(2);
        break;

    case WM_MOUSEMOVE:
        wnd->MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;

    default: break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool Window::Init(const std::string& className)
{
    if (mInitialized)
    {
        LOGD("Window already initialized - ignoring call");
        return true;
    }

    if (className.empty())
    {
        const void* addr = static_cast<const void*>(this);
        std::wstringstream sstream;
        sstream << addr;
        mClassName = L"lkCommon_DefaultClass_" + sstream.str();
    }
    else
    {
        if (!Utils::StringToWString(className, mClassName))
        {
            LOGE("Failed to convert class name string to wstring");
            return false;
        }
    }

    mInstance = GetModuleHandle(0);
    if (!mInstance)
    {
        DWORD error = GetLastError();
        LOGE("Failed to get application's Instance: " << static_cast<int>(error));
        return false;
    }

    WNDCLASSEX wc;
    LKCOMMON_ZERO_MEMORY(wc);
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = mInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = mClassName.c_str();
    if (!RegisterClassEx(&wc))
    {
        DWORD error = GetLastError();
        LOGE("Failed to register application's class: " << static_cast<int>(error));
        return false;
    }

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

    if (mInstance == 0)
    {
        LOGE("WinAPI Instance not acquired - Window not initialized");
        return false;
    }

    // TODO FULLSCREEN
    std::wstring titleWStr;
    if (!Utils::StringToWString(title, titleWStr))
    {
        LOGE("Failed to convert title string to wstring");
        return false;
    }

    RECT wr;
    wr.left = static_cast<LONG>(x);
    wr.right = static_cast<LONG>(x + width);
    wr.top = static_cast<LONG>(y);
    wr.bottom = static_cast<LONG>(y + height);
    AdjustWindowRectEx(&wr, WINDOWED_STYLE, false, WINDOWED_EX_STYLE);

    mHWND = CreateWindowEx(WINDOWED_EX_STYLE, mClassName.c_str(), titleWStr.c_str(), WINDOWED_STYLE,
                           wr.left, wr.top, (wr.right - wr.left), (wr.bottom - wr.top), nullptr, nullptr,
                           mInstance, nullptr);
    if (!mHWND)
    {
        DWORD error = GetLastError();
        LOGE("Failed to create Window: " << static_cast<int>(error));
        return false;
    }

    SetWindowLongPtr(mHWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    UpdateWindow(mHWND);
    if (!mInvisible)
    {
        ShowWindow(mHWND, SW_SHOW);
        SetFocus(mHWND);
    }

    mHDC = GetDC(mHWND);
    if (mHDC == INVALID_HANDLE_VALUE)
    {
        LOGE("Failed to acquire Window's DC");
        return false;
    }

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
        std::wstring wideTitle;
        if (!Utils::StringToWString(title, wideTitle))
        {
            LOGE("Failed to convert title string to wstring");
            return false;
        }

        if (!SetWindowText(mHWND, wideTitle.c_str()))
        {
            DWORD error = GetLastError();
            LOGE("Failed to set title string: " << static_cast<int>(error));
            return false;
        }
    }

    return true;
}

void Window::SetInvisible(const bool invisible)
{
    mInvisible = invisible;

    if (mOpened && mHWND)
    {
        if (mInvisible)
            ShowWindow(mHWND, SW_SHOW);
        else
            ShowWindow(mHWND, SW_HIDE);
    }
}

bool Window::DisplayImage(const uint32_t x, const uint32_t y, const WindowImage& image)
{
    if ((x + image.GetWidth() > mWidth) || (y + image.GetHeight() > mHeight))
    {
        LOGE("Displayed Image extends beyond Window borders");
        return false;
    }

    // WindowImage assumes holding the image from top to bottom, hence -height
    BITMAPINFO bitmapInfo = {0,};
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = static_cast<LONG>(image.GetWidth());
    bitmapInfo.bmiHeader.biHeight = -static_cast<LONG>(image.GetHeight());
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    int res = StretchDIBits(mHDC,
                            x, y, image.GetWidth(), image.GetHeight(),
                            0, 0, image.GetWidth(), image.GetHeight(),
                            image.GetHandle(), &bitmapInfo, DIB_RGB_COLORS,
                            SRCCOPY);
    if (res == 0)
    {
        DWORD err = GetLastError();
        LOGE("Failed to copy image to window: " << static_cast<int>(err));
        return false;
    }

    return true;
}

void Window::Update(const float deltaTime)
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            Close();
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    OnUpdate(deltaTime);
}

void Window::MouseButtonDown(const uint32_t button, const uint32_t x, const uint32_t y)
{
    SetCapture(mHWND);
    mMouseButtons[button] = true;
    mMouseDownX = x;
    mMouseDownY = y;
    OnMouseDown(button);
}

void Window::MouseButtonUp(const uint32_t button)
{
    ReleaseCapture();
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

void Window::Close()
{
    if (!mOpened)
    {
        LOGD("Window is not opened - ignoring call");
        return;
    }

    OnClose();

    if (mHWND != 0)
    {
        if (!DestroyWindow(mHWND))
        {
            LOGE("Failed to destroy window on close");
        }
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

    if (!UnregisterClassW(mClassName.c_str(), mInstance))
    {
        LOGE("Failed to unregister class on deinit");
    }

    mInstance = 0;
    mClassName.clear();
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
