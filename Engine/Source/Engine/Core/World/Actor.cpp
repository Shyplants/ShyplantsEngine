#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/TransformComponent.h"
#include "Engine/Core/Component/RendererComponent.h"

#include "Engine/Graphics/Render/RenderQueue.h"
#include "Engine/Core/Component/CameraComponent2D.h"

// =====================================================
// Constructor / Destructor
// =====================================================

Actor::Actor() = default;
Actor::~Actor() = default;

// =====================================================
// Tick
// =====================================================

void Actor::Tick(float deltaTime)
{
    for (auto& comp : m_components)
    {
        if (comp->IsActive() && comp->IsTickEnabled())
        {
            comp->Tick(deltaTime);
        }
    }
}

// =====================================================
// Destroy
// =====================================================

void Actor::Destroy()
{
    if (m_pendingDestroy)
        return;

    m_pendingDestroy = true;

    // -------------------------------------------------
    // Component Unregister (Áß¿ä)
    // -------------------------------------------------
    for (auto& comp : m_components)
    {
        comp->OnUnregister();
    }

    OnDestroyed();

    if (m_world)
    {
        m_world->DestroyActor(this);
    }
}

// =====================================================
// Root Transform
// =====================================================

void Actor::SetRootTransform(TransformComponent* root)
{
#if defined(_DEBUG)
    SP_ASSERT(m_rootTransform == nullptr);
    SP_ASSERT(root != nullptr);
#endif
    m_rootTransform = root;
}

// =====================================================
// Rendering
// =====================================================

void Actor::SubmitRenderCommands(
    RenderQueue& queue,
    CameraComponent2D& activeCamera)
{
    for (auto& comp : m_components)
    {
        auto* renderer =
            dynamic_cast<RendererComponent*>(comp.get());

        if (!renderer || !renderer->IsVisible())
            continue;

        if (renderer->IsScreenSpace())
        {
            renderer->SubmitUI(queue);
        }
        else
        {
            renderer->SubmitWorld(
                queue,
                activeCamera.GetViewProjectionMatrix());
        }
    }
}
