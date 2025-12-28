#include "Engine/UI/Components/UIComponent.h"

UIComponent::UIComponent(Actor* owner)
	: ActorComponent(owner)
{
}

UIComponent::~UIComponent() = default;