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
// Level
// -------------------------------------------------
#include "Engine/Core/World/GameplayLevel.h"

// -------------------------------------------------
// Game Framework
// -------------------------------------------------
#include "Engine/Core/GameFramework/GameModeBase.h"

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
#include "Engine/Core/Input/InputSystem.h"

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
    if (m_initialized)
        return true;

    void* nativeHandle = window.GetNativeHandle();
    SP_ASSERT(nativeHandle != nullptr);

    // -------------------------------------------------
    // LogSystem (first)
    // -------------------------------------------------
    LogSystem::Init(L"Logs/ShyplantsEngine.log");
    LogSystem::SetGlobalLevel(ELogLevel::Trace);

    SP_LOG(LogCore, ELogLevel::Info,
        "Engine initialization started");

    // -------------------------------------------------
    // InputSystem
    // -------------------------------------------------
    InputSystem::Initialize();

    // -------------------------------------------------
    // ResourceManager
    // -------------------------------------------------
    ResourceManager::Create();

    // -------------------------------------------------
    // GraphicsSubsystem
    // -------------------------------------------------
    m_graphics = std::make_unique<GraphicsSubsystem>();
    if (!m_graphics->Initialize(
        nativeHandle,
        window.GetWidth(),
        window.GetHeight()))
    {
        SP_LOG(LogCore, ELogLevel::Fatal,
            "GraphicsSubsystem initialization failed");
        return false;
    }

    // -------------------------------------------------
    // ResourceLoadContext
    // -------------------------------------------------
    ResourceLoadContext loadContext{};
    loadContext.renderDevice = &m_graphics->GetRenderDevice();
    ResourceManager::Get().SetLoadContext(loadContext);

    // -------------------------------------------------
    // World
    // -------------------------------------------------
    m_world = std::make_unique<World>(
        &m_graphics->GetRenderSystem());

    m_world->OnViewportResized(
        window.GetWidth(),
        window.GetHeight());

    m_initialized = true;

#if defined(_DEBUG)
    SP_LOG(LogCore, ELogLevel::Trace,
        "Engine initialized successfully");
#endif

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
    if (m_world)
    {
        m_world->Shutdown();
        m_world.reset();
    }

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
    SP_ASSERT(m_graphics);
    SP_ASSERT(m_world);

    // -------------------------------------------------
    // Input begin
    // -------------------------------------------------
    InputSystem::BeginFrame();

    // -------------------------------------------------
    // Time
    // -------------------------------------------------
    const double now = Platform::GetTimeSeconds();
    m_time.Update(now);
    const float deltaTime = m_time.GetDeltaTime();

    static const float clearColor[4] =
    {
        0.2f, 0.2f, 0.2f, 1.0f
    };

    // -------------------------------------------------
    // Frame begin
    // -------------------------------------------------
    m_graphics->BeginFrame(clearColor);

    // -------------------------------------------------
    // World update
    // -------------------------------------------------
    m_world->Tick(deltaTime);
    m_world->SubmitRenderCommands();

    // -------------------------------------------------
    // Render
    // -------------------------------------------------
    m_graphics->Render();

    // -------------------------------------------------
    // Frame end
    // -------------------------------------------------
    m_graphics->EndFrame();

    // -------------------------------------------------
    // Input end
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
// Game Setup (Facade)
// =========================================================

void Engine::SetGameMode(std::unique_ptr<GameModeBase> gameMode)
{
    SP_ASSERT(m_initialized);
    SP_ASSERT(m_world);

    m_world->SetGameMode(std::move(gameMode));

    // Level + GameMode 준비 완료 시점
    m_world->StartGameplay();
}

void Engine::LoadGameplayLevel(std::unique_ptr<GameplayLevel> level)
{
    SP_ASSERT(m_initialized);
    SP_ASSERT(m_world);

    m_world->LoadGameplayLevel(std::move(level));
}

// =========================================================
// Accessors
// =========================================================

GraphicsSubsystem& Engine::GetGraphics()
{
    SP_ASSERT(m_graphics);
    return *m_graphics;
}

World& Engine::GetWorld()
{
    SP_ASSERT(m_world);
    return *m_world;
}
