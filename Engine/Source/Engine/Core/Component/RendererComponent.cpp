#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"

RendererComponent::RendererComponent(Actor* owner)
	: ActorComponent(owner)
{
}

RendererComponent::~RendererComponent()
{
}

SceneComponent* RendererComponent::GetSceneComponent() const
{
	Actor* owner = GetOwner();
	if (!owner)
		return nullptr;

	return owner->GetRootComponent();
}