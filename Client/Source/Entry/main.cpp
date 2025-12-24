#include <Windows.h>
#include <ShellScalingApi.h>
#include <dwmapi.h>

#include "Engine/Core/Engine.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Platform/Window/Window.h"
#include "Engine/Platform/Window/WindowDesc.h"

#include "Engine/Core/World/World.h"
#include "Game/Levels/DemoLevel.h"

#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Dwmapi.lib")

void DisableWindowRoundedCorners(HWND hwnd)
{
    constexpr DWORD DWMWA_WINDOW_CORNER_PREFERENCE = 33;

    enum DWM_WINDOW_CORNER_PREFERENCE
    {
        DWMWCP_DEFAULT = 0,
        DWMWCP_DONOTROUND = 1,
        DWMWCP_ROUND = 2,
        DWMWCP_ROUNDSMALL = 3
    };

    DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;

    DwmSetWindowAttribute(
        hwnd,
        DWMWA_WINDOW_CORNER_PREFERENCE,
        &preference,
        sizeof(preference)
    );
}

void LoadInitialLevel(Engine& engine)
{
    auto& world = engine.GetWorld();

    world.LoadGameplayLevel(std::make_unique<DemoLevel>());
}

int WINAPI wWinMain(
    HINSTANCE,
    HINSTANCE,
    PWSTR,
    int)
{
    // -------------------------------------------------
    // DPI Awareness (Per-monitor v2)
    // -------------------------------------------------
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
     
    // -------------------------------------------------
    // Create Window
    // -------------------------------------------------
    auto window = Platform::CreatePlatformWindow();

    WindowDesc desc{};
    desc.Width = 1920;
    desc.Height = 1080;
    desc.Title = L"ShyplantsEngine";

    window->Create(desc);

    // Windows 11: Disable rounded corners
    DisableWindowRoundedCorners(static_cast<HWND>(window->GetNativeHandle()));
        

    // -------------------------------------------------
    // Engine
    // -------------------------------------------------
    Engine engine;

    window->SetResizeCallback(
        [&](uint32 w, uint32 h)
        {
            engine.Resize(w, h);
        });

    if (!engine.Initialize(*window))
    {
        return -1;
    }

    LoadInitialLevel(engine);

    // -------------------------------------------------
    // Main loop
    // -------------------------------------------------
    while (!window->ShouldClose())
    {
        window->PollEvents();
        engine.Tick();
    }

    engine.Shutdown();
    window->Destroy();

    return 0;
}
