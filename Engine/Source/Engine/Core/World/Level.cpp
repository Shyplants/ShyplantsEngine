#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"

#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"

#include "Engine/Graphics/Render/RenderQueue.h"

Level::Level() = default;

Level::~Level() = default;

// =====================================================
// Lifecycle
// =====================================================

void Level::OnEnter(World& world)
{
    SP_ASSERT(m_world == nullptr);
    m_world = &world;
}

void Level::OnExit(World& /*world*/)
{
    // GameplayLevel 교체 시 호출
    m_hasBegunPlay = false;
    m_world = nullptr;
}

// =====================================================
// Shutdown (World 종료)
// =====================================================

void Level::Shutdown()
{
    if (m_isShuttingDown)
        return;

    m_isShuttingDown = true;

    DestroyAllActors();

    m_actors.clear();
    m_destroyQueue.clear();

    m_hasBegunPlay = false;
    m_world = nullptr;
}

// =====================================================
// BeginPlay / Tick
// =====================================================

void Level::OnBeginPlay()
{
    if (m_hasBegunPlay)
        return;

    m_hasBegunPlay = true;

    for (auto& actor : m_actors)
    {
        actor->BeginPlay();
    }
}

void Level::Tick(float deltaTime)
{
    if (!m_hasBegunPlay || !m_world || m_isShuttingDown)
        return;

    ProcessDestroyedActors();

    for (auto& actor : m_actors)
    {
        if (!actor->IsPendingDestroy())
        {
            actor->Tick(deltaTime);
        }
    }
}

// =====================================================
// Rendering
// =====================================================

void Level::SubmitRenderCommands(
    RenderQueue& queue,
    const CameraComponent2D& camera)
{
    if (!m_world || !m_hasBegunPlay || m_isShuttingDown)
        return;

    SubmitWorldRenderers(queue, camera);
    SubmitUIRenderers(queue);
}

// =====================================================
// Actor Management
// =====================================================

Actor* Level::SpawnActorInternal(std::unique_ptr<Actor> actor)
{
    if (m_isShuttingDown)
        return nullptr;

    SP_ASSERT(m_world != nullptr);
    SP_ASSERT(actor != nullptr);

    Actor* rawPtr = actor.get();

    rawPtr->SetWorld(m_world);
    rawPtr->OnSpawned();

    m_actors.push_back(std::move(actor));

    if (m_hasBegunPlay)
    {
        rawPtr->BeginPlay();
    }

    return rawPtr;
}

void Level::MarkActorForDestroy(Actor* actor)
{
    if (!actor || actor->IsPendingDestroy() || m_isShuttingDown)
        return;

    actor->Destroy();
    m_destroyQueue.push_back(actor);
}

void Level::ProcessDestroyedActors()
{
    if (m_destroyQueue.empty())
        return;

    m_actors.erase(
        std::remove_if(
            m_actors.begin(),
            m_actors.end(),
            [](const std::unique_ptr<Actor>& actor)
            {
                return actor->IsPendingDestroy();
            }),
        m_actors.end());

    m_destroyQueue.clear();
}

void Level::DestroyAllActors()
{
    for (auto& actor : m_actors)
    {
        if (actor && !actor->IsPendingDestroy())
        {
            actor->Destroy();
        }
    }

    m_actors.clear();
}

// =====================================================
// Render internals
// =====================================================

void Level::SubmitWorldRenderers(
    RenderQueue& queue,
    const CameraComponent2D& camera)
{
    const auto& viewProj = camera.GetViewProjectionMatrix();

    for (auto& actor : m_actors)
    {
        if (actor->IsPendingDestroy())
            continue;

        for (const auto& comp : actor->GetComponents())
        {
            auto* rc = dynamic_cast<RendererComponent*>(comp.get());
            if (!rc || !rc->IsVisible())
                continue;

            if (rc->IsUIRenderer())
                continue;

            rc->SubmitWorld(queue, viewProj);
        }
    }
}

void Level::SubmitUIRenderers(RenderQueue& queue)
{
    for (auto& actor : m_actors)
    {
        if (actor->IsPendingDestroy())
            continue;

        for (const auto& comp : actor->GetComponents())
        {
            auto* rc = dynamic_cast<RendererComponent*>(comp.get());
            if (!rc || !rc->IsVisible())
                continue;

            if (!rc->IsUIRenderer())
                continue;

            rc->SubmitUI(queue);
        }
    }
}

// =====================================================
// Accessor
// =====================================================

World& Level::GetWorld() const
{
    SP_ASSERT(m_world != nullptr);
    return *m_world;
}
