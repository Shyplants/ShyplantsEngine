#include "Engine/PCH/EnginePCH.h"

#include "Engine/Platform/Windows/WindowsWindow.h"
#include "Engine/Platform/Windows/WindowsInput.h"

#include <Windows.h>

static const wchar_t* WINDOW_CLASS_NAME = L"ShyplantsEngineWindowClass";

// =========================================================
// Constructor / Destructor
// =========================================================

WindowsWindow::WindowsWindow() = default;

WindowsWindow::~WindowsWindow()
{
    Destroy();
}

// =========================================================
// Create / Destroy
// =========================================================

bool WindowsWindow::Create(const WindowDesc& desc)
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // -------------------------------------------------
    // Register window class (once)
    // -------------------------------------------------
    static bool s_registered = false;
    if (!s_registered)
    {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowsWindow::WndProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = WINDOW_CLASS_NAME;

        if (!RegisterClassExW(&wc))
            return false;

        s_registered = true;
    }

    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!desc.bResizable)
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

    RECT rect{};
    rect.left = 0;
    rect.top = 0;
    rect.right = static_cast<LONG>(desc.Width);
    rect.bottom = static_cast<LONG>(desc.Height);

    AdjustWindowRect(&rect, style, FALSE);

    m_hwnd = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        desc.Title,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        this // user data
    );

    if (!m_hwnd)
        return false;

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    m_width = desc.Width;
    m_height = desc.Height;

    return true;
}

void WindowsWindow::Destroy()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

// =========================================================
// Event loop
// =========================================================

void WindowsWindow::PollEvents()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// =========================================================
// WndProc
// =========================================================

LRESULT CALLBACK WindowsWindow::WndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    WindowsWindow* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<WindowsWindow*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(window));
        window->m_hwnd = hwnd;
    }
    else
    {
        window = reinterpret_cast<WindowsWindow*>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window)
    {
        window->HandleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// =========================================================
// Message handling
// =========================================================

void WindowsWindow::HandleMessage(
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    WindowsInput::HandleMessage(msg, wParam, lParam);

    switch (msg)
    {
    case WM_CLOSE:
        m_shouldClose = true;
        break;

    case WM_SIZE:
    {
        if (wParam == SIZE_MINIMIZED)
        {
            m_minimized = true;
        }
        else
        {
            m_minimized = false;
            m_width = LOWORD(lParam);
            m_height = HIWORD(lParam);

            NotifyResize(m_width, m_height);
        }
        break;
    }

    case WM_DESTROY:
        m_shouldClose = true;
        PostQuitMessage(0);
        break;
    }
}
