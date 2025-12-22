#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"

#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"

#include "Engine/Graphics/Render/RenderQueue.h"

// =====================================================
// Lifecycle
// =====================================================

Level::~Level() = default;

void Level::OnEnter(World& world)
{
    m_world = &world;
}

void Level::OnExit(World& /*world*/)
{
    m_actors.clear();
    m_destroyQueue.clear();
    m_world = nullptr;
    m_hasBegunPlay = false;
}

void Level::OnBeginPlay()
{
    m_hasBegunPlay = true;

    for (auto& actor : m_actors)
    {
        actor->BeginPlay();
    }
}

// =====================================================
// Tick
// =====================================================

void Level::Tick(float deltaTime)
{
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
// Rendering (Submit only)
// =====================================================

void Level::SubmitRenderCommands(
    RenderQueue& queue,
    const CameraComponent2D& camera)
{
    SubmitWorldRenderers(queue, camera);
    SubmitUIRenderers(queue);
}

// =====================================================
// Actor Management
// =====================================================

Actor* Level::SpawnActorInternal(std::unique_ptr<Actor> actor)
{
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
    if (!actor || actor->IsPendingDestroy())
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
        m_actors.end()
    );

    m_destroyQueue.clear();
}

// =====================================================
// Render submission internals
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
            auto* rc =
                dynamic_cast<RendererComponent*>(comp.get());

            if (!rc || !rc->IsVisible() || rc->IsUIRenderer())
                continue;

            rc->SubmitWorld(queue, viewProj);
        }
    }
}

void Level::SubmitUIRenderers(
    RenderQueue& queue)
{
    for (auto& actor : m_actors)
    {
        if (actor->IsPendingDestroy() || !actor->IsUIActor())
            continue;

        for (const auto& comp : actor->GetComponents())
        {
            auto* rc =
                dynamic_cast<RendererComponent*>(comp.get());

            if (!rc || !rc->IsVisible() || !rc->IsUIRenderer())
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
