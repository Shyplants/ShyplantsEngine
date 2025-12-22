#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"

RendererComponent::RendererComponent(Actor* owner)
    : ActorComponent(owner)
{
}

void RendererComponent::OnRegister()
{
    ActorComponent::OnRegister();
}
