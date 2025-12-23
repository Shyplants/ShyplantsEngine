#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Engine.h"

// -------------------------------------------------
// Log
// -------------------------------------------------
#include "Engine/Core/Log/LogSystem.h"

// -------------------------------------------------
// Subsystems
// -------------------------------------------------
#include "Engine/Graphics/Subsystem/GraphicsSubsystem.h"

// -------------------------------------------------
// World
// -------------------------------------------------
#include "Engine/Core/World/World.h"

// -------------------------------------------------
// Resource
// -------------------------------------------------
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/ResourceLoadContext.h"

// -------------------------------------------------
// Platform
// -------------------------------------------------
#include "Engine/Platform/Platform.h"
#include "Engine/Platform/Window/Window.h"

// -------------------------------------------------
// Input
// -------------------------------------------------
#include "Engine/Platform/Input/InputSystem.h"

// =========================================================
// Constructor / Destructor
// =========================================================

Engine::Engine() = default;

Engine::~Engine()
{
    Shutdown();
}

// =========================================================
// Initialize
// =========================================================

bool Engine::Initialize(Window& window)
{
    SP_ASSERT(!m_initialized);

    void* native = window.GetNativeHandle();

    // -------------------------------------------------
    // LogSystem (Global, first)
    // -------------------------------------------------
    LogSystem::Init(L"Logs/ShyplantsEngine.log");
    LogSystem::SetGlobalLevel(ELogLevel::Trace);

    SP_LOG(LogCore, ELogLevel::Info,
        "Engine initialization started");

    // -------------------------------------------------
    // InputSystem (Global)
    // -------------------------------------------------
    InputSystem::Initialize();

    // -------------------------------------------------
    // ResourceManager (Global)
    // -------------------------------------------------
    ResourceManager::Create();

    // -------------------------------------------------
    // GraphicsSubsystem
    // -------------------------------------------------
    m_graphics = std::make_unique<GraphicsSubsystem>();
    if (!m_graphics->Initialize(native, window.GetWidth(), window.GetHeight()))
    {
        SP_LOG(LogCore, ELogLevel::Fatal,
            "Engine: GraphicsSubsystem initialization failed");
        return false;
    }

    // -------------------------------------------------
    // Setup ResourceLoadContext
    // -------------------------------------------------
    ResourceLoadContext context{};
    context.renderDevice = &m_graphics->GetRenderDevice();
    ResourceManager::Get().SetLoadContext(context);

    // -------------------------------------------------
    // World
    // -------------------------------------------------
    m_world = std::make_unique<World>(
        &m_graphics->GetRenderSystem());

    SP_ASSERT(m_world != nullptr);
    m_world->OnViewportResized(window.GetWidth(), window.GetHeight());

#if defined(_DEBUG)
    SP_LOG(LogCore, ELogLevel::Trace,
        "Engine initialized successfully");
#endif

    m_initialized = true;
    return true;
}

// =========================================================
// Shutdown
// =========================================================

void Engine::Shutdown()
{
    if (!m_initialized)
        return;

    SP_LOG(LogCore, ELogLevel::Info,
        "Engine shutdown started");

    // -------------------------------------------------
    // World
    // -------------------------------------------------
    m_world->Shutdown();

    // -------------------------------------------------
    // Graphics
    // -------------------------------------------------
    if (m_graphics)
    {
        m_graphics->Shutdown();
        m_graphics.reset();
    }

    // -------------------------------------------------
    // ResourceManager
    // -------------------------------------------------
    ResourceManager::Destroy();

    // -------------------------------------------------
    // InputSystem
    // -------------------------------------------------
    InputSystem::Shutdown();

#if defined(_DEBUG)
    SP_LOG(LogCore, ELogLevel::Trace,
        "Engine shutdown completed");
#endif

    // -------------------------------------------------
    // LogSystem (last)
    // -------------------------------------------------
    LogSystem::Shutdown();

    m_initialized = false;
}

// =========================================================
// Tick
// =========================================================

void Engine::Tick()
{
    SP_ASSERT(m_initialized);
    SP_ASSERT(m_graphics != nullptr);
    SP_ASSERT(m_world != nullptr);

    // -------------------------------------------------
    // Input frame begin
    // -------------------------------------------------
    InputSystem::BeginFrame();

    // -------------------------------------------------
    // Time
    // -------------------------------------------------
    double now = Platform::GetTimeSeconds();
    m_time.Update(now);
    float deltaTime = m_time.GetDeltaTime();

    static const float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

    // -------------------------------------------------
    // Begin Frame
    // -------------------------------------------------
    m_graphics->BeginFrame(clearColor);

    // -------------------------------------------------
    // Update world
    // -------------------------------------------------
    m_world->Tick(deltaTime);
    m_world->SubmitRenderCommands();

    // -------------------------------------------------
    // Render
    // -------------------------------------------------
    m_graphics->Render();

    // -------------------------------------------------
    // End Frame
    // -------------------------------------------------
    m_graphics->EndFrame();

    // -------------------------------------------------
    // Input frame end
    // -------------------------------------------------
    InputSystem::EndFrame();
}

// =========================================================
// Resize
// =========================================================

void Engine::Resize(uint32 width, uint32 height)
{
    if (!m_initialized || !m_graphics)
        return;

    m_graphics->Resize(width, height);

    if (m_world)
    {
        m_world->OnViewportResized(width, height);
    }
}

// =========================================================
// Accessors
// =========================================================

GraphicsSubsystem& Engine::GetGraphics()
{
    SP_ASSERT(m_graphics != nullptr);
    return *m_graphics;
}

World& Engine::GetWorld()
{
    SP_ASSERT(m_world != nullptr);
    return *m_world;
}
