#pragma once

#include <memory>
#include <Windows.h>

// Forward declarations
class Engine;
class Win32Window;

/*
    Win32Application
    -------------------------------------------------
    - Client 실행 정책 담당
    - Win32 메시지 루프
    - DeltaTime 계산
    - Engine 수명 관리
*/
class Win32Application
{
public:
    Win32Application();
    ~Win32Application();

    Win32Application(const Win32Application&) = delete;
    Win32Application& operator=(const Win32Application&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    bool Initialize(
        HINSTANCE hInstance,
        int width,
        int height);

    int Run();

private:
    // =====================================================
    // Internals
    // =====================================================
    void ProcessMessages();
    float CalculateDeltaTime();

private:
    bool m_running{ true };

    std::unique_ptr<Win32Window> m_window;
    std::unique_ptr<Engine>      m_engine;

    // High-resolution timer
    LARGE_INTEGER m_frequency{};
    LARGE_INTEGER m_prevCounter{};
};
