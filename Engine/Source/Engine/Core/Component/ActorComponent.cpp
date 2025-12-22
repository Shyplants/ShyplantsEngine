#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"

ActorComponent::ActorComponent(Actor* owner)
    : m_owner(owner)
{
}

ActorComponent::~ActorComponent()
{
    OnUnregister();
    OnDestroy();
}

World* ActorComponent::GetWorld() const
{
    return m_world ? m_world : (m_owner ? m_owner->GetWorld() : nullptr);
}
