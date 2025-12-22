#include "Win32Window.h"

LRESULT CALLBACK Win32Window::WndProc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Win32Window::Create(
    HINSTANCE hInstance,
    int width,
    int height,
    const wchar_t* title)
{
    WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ShyplantsClientWindow";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassEx(&wc);

    // -------------------------------------------------
    // DPI-aware window size
    // -------------------------------------------------
    RECT rect{ 0, 0, width, height };
    AdjustWindowRectEx(
        &rect,
        WS_OVERLAPPEDWINDOW,
        FALSE,
        0);

    m_hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if (!m_hwnd)
        return false;

    ShowWindow(m_hwnd, SW_SHOW);
    return true;
}
