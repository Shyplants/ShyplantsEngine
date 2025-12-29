#include "Engine/UI/Component/UIComponent.h"

UIComponent::UIComponent(Actor* owner)
	: ActorComponent(owner)
{
}

UIComponent::~UIComponent() = default;