#include "Application/Win32Application.h"

#include "Engine/Core/Engine.h"
#include "Platform/Win32/Win32Window.h"

#include "Game/GameBootstrap.h"

// =====================================================
// Destructor
// =====================================================

Win32Application::Win32Application() = default;

Win32Application::~Win32Application()
{
    m_engine.reset();
}

// =====================================================
// Initialize
// =====================================================

bool Win32Application::Initialize(
    HINSTANCE hInstance,
    int width,
    int height)
{
    // -------------------------------------------------
    // Create Window
    // -------------------------------------------------
    m_window = std::make_unique<Win32Window>();
    if (!m_window->Create(
        hInstance,
        width,
        height,
        L"ShyplantsEngine Client"))
    {
        return false;
    }

    // -------------------------------------------------
    // Create Engine
    // -------------------------------------------------
    m_engine = std::make_unique<Engine>();
    if (!m_engine->Initialize(
        m_window->GetHWND(),
        static_cast<uint32>(width),
        static_cast<uint32>(height)))
    {
        return false;
    }

    // -------------------------------------------------
    // Initialize high-resolution timer
    // -------------------------------------------------
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_prevCounter);


    Game::LoadInitialLevel(*m_engine);

    return true;
}

// =====================================================
// Run (Main Loop)
// =====================================================

int Win32Application::Run()
{
    MSG msg{};

    while (m_running)
    {
        ProcessMessages();
        if (!m_running)
            break;

        const float deltaTime = CalculateDeltaTime();
        m_engine->Tick();
    }

    return static_cast<int>(msg.wParam);
}

// =====================================================
// Message Processing
// =====================================================

void Win32Application::ProcessMessages()
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_running = false;
            return;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// =====================================================
// Delta Time
// =====================================================

float Win32Application::CalculateDeltaTime()
{
    LARGE_INTEGER current{};
    QueryPerformanceCounter(&current);

    const float delta =
        static_cast<float>(current.QuadPart - m_prevCounter.QuadPart) /
        static_cast<float>(m_frequency.QuadPart);

    m_prevCounter = current;
    return delta;
}
