#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Level.h"

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"

#include "Engine/Graphics/Render/RenderQueue.h"

// =====================================================
// Constructor / Destructor
// =====================================================

Actor::Actor()
{
    m_rootComponent = AddComponent<SceneComponent>();
    SetRootComponent(m_rootComponent);
}

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

    for (Actor* child : m_childActors)
    {
        if (child && !child->IsPendingDestroy())
        {
            child->Destroy();
        }
    }

    OnDestroyed();

    if (m_world)
    {
        m_world->DestroyActor(this);
    }
}

// =====================================================
// Root Component
// =====================================================

void Actor::SetRootComponent(SceneComponent* newRoot)
{
    if (!newRoot || newRoot == m_rootComponent)
        return;

    SceneComponent* oldRoot = m_rootComponent;
    m_rootComponent = newRoot;

    // Scene hierarchy migration
    if (oldRoot)
    {
        auto children = oldRoot->GetChildren();
        for (SceneComponent* child : children)
        {
            child->DetachUnsafe(FDetachmentTransformRules::KeepWorldTransform);
            child->AttachToUnsafe(newRoot, FAttachmentTransformRules::KeepWorldTransform);
        }
    }

    if (m_rootComponent->GetParent())
    {
        m_rootComponent->DetachUnsafe(FDetachmentTransformRules::KeepWorldTransform);
    }
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

        if (!renderer)
            continue;

        renderer->Submit(queue, &activeCamera);
    }
}

// =====================================================
// Actor Hierarchy
// =====================================================

void Actor::AttachToActor(
    Actor* parent,
    const FAttachmentTransformRules& rules)
{
    if (!parent || parent == this)
        return;

    if (m_parentActor)
    {
        DetachFromParent();
    }

    m_parentActor = parent;
    parent->m_childActors.push_back(this);

    if (m_rootComponent && parent->m_rootComponent)
    {
        m_rootComponent->AttachToUnsafe(parent->m_rootComponent, rules);
    }
}

void Actor::DetachFromParent(
    const FDetachmentTransformRules& rules)
{
    if (!m_parentActor)
        return;

    auto& siblings = m_parentActor->m_childActors;
    siblings.erase(
        std::remove(siblings.begin(), siblings.end(), this),
        siblings.end());

    if (m_rootComponent)
    {
        m_rootComponent->DetachUnsafe(rules);
    }

    m_parentActor = nullptr;
}
