#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/PersistentLevel.h"
#include "Engine/Core/World/GameplayLevel.h"

#include "Engine/Core/Component/CameraComponent2D.h"

#include "Engine/Core/GameFramework/GameModeBase.h"
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

    // PersistentLevel은 World 수명 동안 유지
    m_persistentLevel = std::make_unique<PersistentLevel>();
    m_persistentLevel->OnEnter(*this);
    m_persistentLevel->OnBeginPlay();
}

World::~World()
{
    // Shutdown은 외부에서 명시적으로 호출되어야 함
}

// =====================================================
// Shutdown
// =====================================================

void World::Shutdown()
{
    if (m_isShuttingDown)
        return;

    m_isShuttingDown = true;

    // 1. GameMode 종료
    if (m_gameMode)
    {
        m_gameMode->OnEndPlay();
        m_gameMode.reset();
    }

    // 2. GameplayLevel 종료
    ShutdownGameplayLevel();

    // 3. PersistentLevel 종료
    if (m_persistentLevel)
    {
        m_persistentLevel->Shutdown();
        m_persistentLevel.reset();
    }

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

    if (m_persistentLevel)
        m_persistentLevel->Tick(deltaTime);

    if (m_gameplayLevel)
        m_gameplayLevel->Tick(deltaTime);
}

// =====================================================
// Render Submit
// =====================================================

void World::SubmitRenderCommands()
{
    if (m_isShuttingDown || !m_activeCamera)
        return;

    RenderQueue& queue = m_renderSystem->GetRenderQueue();

    if (m_gameplayLevel)
        m_gameplayLevel->SubmitRenderCommands(queue, *m_activeCamera);

    if (m_persistentLevel)
        m_persistentLevel->SubmitRenderCommands(queue, *m_activeCamera);
}

// =====================================================
// Actor Management
// =====================================================

void World::DestroyActor(Actor* actor)
{
    if (!actor || m_isShuttingDown)
        return;

    NotifyActorDestroyed(actor);

    Level* ownerLevel =
        actor->IsPersistentActor()
        ? static_cast<Level*>(m_persistentLevel.get())
        : static_cast<Level*>(m_gameplayLevel.get());

    if (ownerLevel)
    {
        ownerLevel->MarkActorForDestroy(actor);
    }
}

Actor* World::SpawnActor_Internal(std::unique_ptr<Actor> actor)
{
    SP_ASSERT(actor != nullptr);

    Level* targetLevel =
        actor->IsPersistentActor()
        ? static_cast<Level*>(m_persistentLevel.get())
        : static_cast<Level*>(m_gameplayLevel.get());

    if (!targetLevel)
        return nullptr;

    return targetLevel->SpawnActorInternal(std::move(actor));
}

// =====================================================
// Level Management
// =====================================================

void World::LoadGameplayLevel(std::unique_ptr<GameplayLevel> level)
{
    SP_ASSERT(level != nullptr);

    ShutdownGameplayLevel();

    m_gameplayLevel = std::move(level);
    m_gameplayLevel->OnEnter(*this);
    m_gameplayLevel->OnBeginPlay();
}

void World::ShutdownGameplayLevel()
{
    if (!m_gameplayLevel)
        return;

    m_gameplayLevel->Shutdown();
    m_gameplayLevel.reset();
}

// =====================================================
// Game Framework
// =====================================================

void World::SetGameMode(std::unique_ptr<GameModeBase> gameMode)
{
    SP_ASSERT(!m_isShuttingDown);

    if (m_gameMode)
    {
        m_gameMode->OnEndPlay();
        m_gameMode.reset();
    }

    m_gameMode = std::move(gameMode);

    if (m_gameMode)
    {
        m_gameMode->Initialize();
        m_gameMode->OnBeginPlay();
    }
}

GameState* World::GetGameState() const
{
    return m_gameMode ? m_gameMode->GetGameState() : nullptr;
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
// Rendering Access
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
