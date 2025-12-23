#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/Actor.h"

#include "Engine/Core/Component/CameraComponent2D.h"

#include "Engine/Core/GameFramework/GameMode.h"
#include "Engine/Core/GameFramework/GameState.h"

#include "Engine/Graphics/Render/RenderSystem.h"
#include "Engine/Graphics/Render/RenderQueue.h"

// =====================================================
// Constructor / Destructor
// =====================================================

World::World(RenderSystem* renderSystem)
    : m_renderSystem(renderSystem)
{
    SP_ASSERT(m_renderSystem != nullptr);
    CreateGameFramework();
}

World::~World()
{
    // Shutdown은 반드시 명시적으로 호출되어야 한다
}

// =====================================================
// Shutdown
// =====================================================

void World::Shutdown()
{
    if (m_isShuttingDown)
        return;

    m_isShuttingDown = true;

    // -------------------------------------------------
    // 1. Gameplay 종료
    // -------------------------------------------------
    if (m_gameMode)
    {
        m_gameMode->OnEndPlay();
    }

    // -------------------------------------------------
    // 2. Level Shutdown (Actor Destroy)
    // -------------------------------------------------
    ShutdownCurrentLevel();

    // -------------------------------------------------
    // 3. GameFramework 정리
    // -------------------------------------------------
    m_gameMode.reset();

    m_activeCamera = nullptr;
}

// =====================================================
// Tick
// =====================================================

void World::Tick(float deltaTime)
{
    if (m_isShuttingDown)
        return;

    if (m_gameMode)
        m_gameMode->Tick(deltaTime);

    if (m_currentLevel)
        m_currentLevel->Tick(deltaTime);
}

// =====================================================
// Render Submit
// =====================================================

void World::SubmitRenderCommands()
{
    if (m_isShuttingDown || !m_currentLevel || !m_activeCamera)
        return;

    RenderQueue& queue = m_renderSystem->GetRenderQueue();

    m_currentLevel->SubmitRenderCommands(
        queue,
        *m_activeCamera);
}

// =====================================================
// Actor Management
// =====================================================

void World::DestroyActor(Actor* actor)
{
    if (!actor || !m_currentLevel || m_isShuttingDown)
        return;

    NotifyActorDestroyed(actor);
    m_currentLevel->MarkActorForDestroy(actor);
}

Actor* World::SpawnActor_Impl(std::unique_ptr<Actor> actor)
{
    return m_currentLevel
        ? m_currentLevel->SpawnActorInternal(std::move(actor))
        : nullptr;
}

// =====================================================
// Level Management
// =====================================================

void World::LoadLevel(std::unique_ptr<Level> level)
{
    SP_ASSERT(level != nullptr);

    UnloadCurrentLevel();

    m_currentLevel = std::move(level);
    m_currentLevel->OnEnter(*this);

    if (m_gameMode)
        m_gameMode->OnBeginPlay();

    m_currentLevel->OnBeginPlay();
}

void World::UnloadCurrentLevel()
{
    if (!m_currentLevel)
        return;

    m_currentLevel->OnExit(*this);
    m_currentLevel.reset();

    m_activeCamera = nullptr;
}

void World::ShutdownCurrentLevel()
{
    if (!m_currentLevel)
        return;

    m_currentLevel->Shutdown();
    m_currentLevel.reset();
}

// =====================================================
// Game Framework
// =====================================================

GameState* World::GetGameState() const
{
    return m_gameMode ? m_gameMode->GetGameState() : nullptr;
}

void World::SetGameMode(std::unique_ptr<GameMode> gameMode)
{
    if (m_gameMode)
    {
        m_gameMode->OnEndPlay();
    }

    m_gameMode = std::move(gameMode);
}

// =====================================================
// Camera
// =====================================================

void World::SetActiveCamera(CameraComponent2D* camera)
{
    if (!camera)
    {
        m_activeCamera = nullptr;
        return;
    }

    Actor* owner = camera->GetOwner();
    SP_ASSERT(owner != nullptr);
    SP_ASSERT(owner->GetWorld() == this);

    m_activeCamera = camera;

    if (m_lastViewportW && m_lastViewportH)
    {
        m_activeCamera->SetViewSize(
            static_cast<float>(m_lastViewportW),
            static_cast<float>(m_lastViewportH));
    }
}

void World::OnViewportResized(uint32 width, uint32 height)
{
    m_lastViewportW = width;
    m_lastViewportH = height;

    if (m_activeCamera)
    {
        m_activeCamera->SetViewSize(
            static_cast<float>(width),
            static_cast<float>(height));
    }
}

void World::NotifyActorDestroyed(Actor* actor)
{
    if (m_activeCamera && m_activeCamera->GetOwner() == actor)
    {
        m_activeCamera = nullptr;
    }
}

// =====================================================
// Rendering access
// =====================================================

RenderQueue& World::GetRenderQueue() const
{
    SP_ASSERT(m_renderSystem != nullptr);
    return m_renderSystem->GetRenderQueue();
}

RenderSystem& World::GetRenderSystem() const
{
    SP_ASSERT(m_renderSystem != nullptr);
    return *m_renderSystem;
}

// =====================================================
// Internal
// =====================================================

void World::CreateGameFramework()
{
    m_gameMode = std::make_unique<GameMode>(*this);
}
