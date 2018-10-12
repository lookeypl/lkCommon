#include "lkCommon/System/Window.hpp"
#include "lkCommon/Utils/Logger.hpp"
#include "lkCommon/Utils/StringConv.hpp"


namespace lkCommon {
namespace System {

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
const DWORD WINDOWED_EX_STYLE = WS_EX_WINDOWEDGE;
const DWORD WINDOWED_STYLE = WS_OVERLAPPEDWINDOW;

Window::Window()
    : mWidth(0)
    , mHeight(0)
    , mMouseDownX(0)
    , mMouseDownY(0)
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

Window::~Window()
{
    Close();

    if (mInstance != 0)
    {
        if (!UnregisterClassW(mClassName.c_str(), mInstance))
        {
            LOGE("Failed to unregister class on close");
        }
    }
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
    if (mInstance)
        return true;

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

    OnInit();

    return true;
}

bool Window::Open(int x, int y, int width, int height, const std::string& title)
{
    if (mOpened)
        return false; // we cannot open a new window - we are already opened

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
    wr.left = (long)x;
    wr.right = x + width;
    wr.top = y;
    wr.bottom = y + height;
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

    OnOpen();

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

void Window::SetInvisible(bool invisible)
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

bool Window::DisplayImage(uint32_t x, uint32_t y, const Utils::Image& image)
{
    if ((x + image.GetWidth() > mWidth) || (y + image.GetHeight() > mHeight))
    {
        LOGE("Displayed Image extends beyond Window borders");
        return false;
    }

    // Utils::Image holds the image from top to bottom, hence -height
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
                            image.GetDataPtr(), &bitmapInfo, DIB_RGB_COLORS,
                            SRCCOPY);
    if (res == 0)
    {
        DWORD err = GetLastError();
        LOGE("Failed to copy image to window: " << static_cast<int>(err));
        return false;
    }

    return true;
}

void Window::Update(float deltaTime)
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

void Window::MouseButtonDown(int button, int x, int y)
{
    SetCapture(mHWND);
    mMouseButtons[button] = true;
    mMouseDownX = x;
    mMouseDownY = y;
    OnMouseDown(button);
}

void Window::MouseButtonUp(int button)
{
    ReleaseCapture();
    mMouseButtons[button] = false;
    OnMouseUp(button);
}

void Window::MouseMove(int x, int y)
{
    OnMouseMove(x, y, x - mMouseDownX, y - mMouseDownY);
    mMouseDownX = x;
    mMouseDownY = y;
}

void Window::Close()
{
    if (!mOpened)
        return;

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

} // namespace System
} // namespace lkCommon
