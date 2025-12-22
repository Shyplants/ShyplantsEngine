#pragma once

#include <Windows.h>

#include "Engine/Platform/Window/Window.h"

struct HWND__;
using HWND = HWND__*;

/*
    WindowsWindow
    -------------------------------------------------
    - Win32 기반 Window 구현체
    - HWND / 메시지 루프 관리
    - Engine/Core는 이 클래스를 직접 참조하지 않음
*/
class WindowsWindow final : public Window
{
public:
    WindowsWindow();
    ~WindowsWindow() override;

    WindowsWindow(const WindowsWindow&) = delete;
    WindowsWindow& operator=(const WindowsWindow&) = delete;

public:
    // =========================================================
    // Window interface
    // =========================================================
    bool Create(const WindowDesc& desc) override;
    void Destroy() override;

    void PollEvents() override;

    uint32 GetWidth() const override { return m_width; }
    uint32 GetHeight() const override { return m_height; }

    bool IsMinimized() const override { return m_minimized; }
    bool ShouldClose() const override { return m_shouldClose; }

    void* GetNativeHandle() const override
    {
        return reinterpret_cast<void*>(m_hwnd);
    }

private:
    // =========================================================
    // Win32
    // =========================================================
    static LRESULT CALLBACK WndProc(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam);

    void HandleMessage(
        UINT msg,
        WPARAM wParam,
        LPARAM lParam);

private:
    HWND   m_hwnd{ nullptr };
    uint32 m_width{ 0 };
    uint32 m_height{ 0 };

    bool m_minimized{ false };
    bool m_shouldClose{ false };
};
