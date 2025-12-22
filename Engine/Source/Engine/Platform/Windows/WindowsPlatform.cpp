#include "Engine/PCH/EnginePCH.h"

#include "Engine/Platform/Platform.h"
#include "Engine/Platform/Windows/WindowsPlatform.h"

#include "Engine/Platform/Windows/WindowsWindow.h"

#include <Windows.h>

// =========================================================
// Platform Facade ¡æ Windows Implementation
// =========================================================

std::unique_ptr<Window> Platform::CreatePlatformWindow()
{
    return std::unique_ptr<Window>(WindowsPlatform::CreatePlatformWindow());
}

double Platform::GetTimeSeconds()
{
    return WindowsPlatform::GetTimeSeconds();
}

// =========================================================
// WindowsPlatform
// =========================================================

Window* WindowsPlatform::CreatePlatformWindow()
{
    return new WindowsWindow();
}

double WindowsPlatform::GetTimeSeconds()
{
    static LARGE_INTEGER frequency{};
    static bool initialized = false;

    if (!initialized)
    {
        QueryPerformanceFrequency(&frequency);
        initialized = true;
    }

    LARGE_INTEGER counter{};
    QueryPerformanceCounter(&counter);

    return static_cast<double>(counter.QuadPart) /
        static_cast<double>(frequency.QuadPart);
}