#pragma once
#include <Windows.h>

class Win32Window
{
public:
    bool Create(
        HINSTANCE hInstance,
        int width,
        int height,
        const wchar_t* title);

    HWND GetHWND() const { return m_hwnd; }

private:
    static LRESULT CALLBACK WndProc(
        HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd{ nullptr };
};
