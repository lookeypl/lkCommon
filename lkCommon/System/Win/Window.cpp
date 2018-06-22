#include "PCH.hpp"

#include "../Window.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UTF.hpp"


namespace lkCommon {
namespace System {

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
const DWORD WINDOWED_EX_STYLE = WS_EX_WINDOWEDGE;
const DWORD WINDOWED_STYLE = WS_OVERLAPPEDWINDOW;

Window::Window()
    : mInstance(0)
    , mHWND(0)
    , mWidth(0)
    , mHeight(0)
    , mOpened(false)
    , mInvisible(false)
{
}

Window::~Window()
{
    Close();
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

bool Window::Init()
{
    if (mInstance)
        return true;

    const void* addr = static_cast<const void*>(this);
    std::wstringstream sstream;
    sstream << addr;
    mClassName = L"lkCommon_Class_" + sstream.str();

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
    std::wstring wideTitle;
    if (!Utils::UTF8ToUTF16(title, wideTitle))
        return false;

    RECT wr;
    wr.left = (long)x;
    wr.right = x + width;
    wr.top = y;
    wr.bottom = y + height;
    AdjustWindowRectEx(&wr, WINDOWED_STYLE, false, WINDOWED_EX_STYLE);

    mHWND = CreateWindowEx(WINDOWED_EX_STYLE, mClassName.c_str(), wideTitle.c_str(), WINDOWED_STYLE,
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

    mOpened = true;
    mWidth = width;
    mHeight = height;

    OnOpen();

    return true;
}

bool Window::SetTitle(const std::wstring& title)
{
    if (!SetWindowText(mHWND, title.c_str()))
    {
        DWORD error = GetLastError();
        LOGE("Failed to set title string: " << static_cast<int>(error));
        return false;
    }

    return true;
}

bool Window::SetTitle(const std::string& title)
{
    // Convert title to UTF16
    std::wstring wideTitle;
    if (!Utils::UTF8ToUTF16(title, wideTitle))
    {
        return false;
    }

    return SetTitle(wideTitle);
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
        LOGE("Displayed Image extens beyond Window borders");
        return false;
    }

    HBITMAP img = CreateBitmap(image.GetWidth(), image.GetHeight(), 1, sizeof(Utils::Image::Pixel) * 8, image.GetDataPtr());
    if (img == INVALID_HANDLE_VALUE)
    {
        LOGE("Failed to create temporary bitmap for displayed image");
        return false;
    }

    HDC tempDC = CreateCompatibleDC(mHDC);
    if (tempDC == INVALID_HANDLE_VALUE)
    {
        LOGE("Failed to obtain a compatible DC for bitmap display");
        DeleteObject(img);
        return false;
    }

    SelectObject(tempDC, img);
    BitBlt(mHDC, x, y, image.GetWidth(), image.GetHeight(), tempDC, 0, 0, SRCCOPY);

    DeleteDC(tempDC);
    DeleteObject(img);
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

    if (mInstance != 0)
    {
        if (mHWND != 0)
            DestroyWindow(mHWND);

        UnregisterClass(mClassName.c_str(), mInstance);
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
