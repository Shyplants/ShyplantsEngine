#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Level.h"

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/Component/RendererComponent.h"

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
// World
// =====================================================

void Actor::SetWorld(World* world)
{
    m_world = world;

    for (auto& comp : m_components)
    {
        comp->SetWorld(world);
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

    if (oldRoot)
    {
        auto children = oldRoot->GetChildren();
        for (SceneComponent* child : children)
        {
            child->Detach(FDetachmentTransformRules::KeepWorldTransform);
            child->AttachTo(newRoot, FAttachmentTransformRules::KeepWorldTransform);
        }
    }

    if (m_rootComponent->GetParent())
    {
        m_rootComponent->Detach(FDetachmentTransformRules::KeepWorldTransform);
    }

    // Renderer rebind (조건부로)
    for (auto& comp : m_components)
    {
        if (auto* renderer = dynamic_cast<RendererComponent*>(comp.get()))
        {
            if (renderer->GetAttachComponent() == oldRoot)
            {
                AttachRendererTo(renderer, newRoot);
            }
        }
    }
}

// =====================================================
// Renderer Attachment (NEW API)
// =====================================================

void Actor::AttachRendererTo(
    RendererComponent* renderer,
    SceneComponent* scene)
{
    SP_ASSERT(renderer);
    SP_ASSERT(scene);

    SP_ASSERT(renderer->GetOwner() == this);
    SP_ASSERT(scene->GetOwner() == this);

    renderer->SetAttachComponent(scene);
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
        m_rootComponent->AttachTo(parent->m_rootComponent, rules);
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
        m_rootComponent->Detach(rules);
    }

    m_parentActor = nullptr;
}
