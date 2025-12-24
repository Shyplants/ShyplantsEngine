#include <Windows.h>
#include <ShellScalingApi.h>

#include "Engine/Core/Engine.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Platform/Window/Window.h"
#include "Engine/Platform/Window/WindowDesc.h"

#include "ClientInit.h"

#pragma comment(lib, "Shcore.lib")

int WINAPI wWinMain(
    HINSTANCE,
    HINSTANCE,
    PWSTR,
    int)
{
    // -------------------------------------------------
    // DPI Awareness
    // -------------------------------------------------
    SetProcessDpiAwarenessContext(
        DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // -------------------------------------------------
    // Window
    // -------------------------------------------------
    auto window = Platform::CreatePlatformWindow();

    WindowDesc desc{};
    desc.Width = 1920;
    desc.Height = 1080;
    desc.Title = L"ShyplantsEngine Client";

    window->Create(desc);

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
        return -1;

    // -------------------------------------------------
    // Client Game Setup
    // -------------------------------------------------
    ClientInit::SetupGame(engine);

    // -------------------------------------------------
    // Main Loop
    // -------------------------------------------------
    while (!window->ShouldClose())
    {
        window->PollEvents();
        engine.Tick();
    }

    // -------------------------------------------------
    // Shutdown
    // -------------------------------------------------
    engine.Shutdown();
    window->Destroy();

    return 0;
}
