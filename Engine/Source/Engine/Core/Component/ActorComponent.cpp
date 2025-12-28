#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"

// =====================================================
// Constructor / Destructor
// =====================================================

ActorComponent::ActorComponent(Actor* owner)
    : m_owner(owner)
{
    SP_ASSERT(owner != nullptr);
}

ActorComponent::~ActorComponent() = default;

// =====================================================
// Lifecycle
// =====================================================

void ActorComponent::OnRegister()
{
}

void ActorComponent::OnUnregister()
{
}

void ActorComponent::OnBeginPlay()
{
}

void ActorComponent::OnEndPlay()
{
}

// =====================================================
// State
// =====================================================

void ActorComponent::SetActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;

    if (m_active)
        OnActivated();
    else
        OnDeactivated();
}

// =====================================================
// Ownership
// =====================================================

World* ActorComponent::GetWorld() const
{
    return m_owner ? m_owner->GetWorld() : nullptr;
}
