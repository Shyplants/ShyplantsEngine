#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/UIActor.h"

#include "Engine/UI/Components/UIScreenRootComponent.h"
#include "Engine/UI/Layout/UIScreenTransform.h"
#include "Engine/Core/World/World.h"

UIActor::UIActor()
    : Actor()
{
}

void UIActor::OnSpawned()
{
    Actor::OnSpawned();

    // -------------------------------------------------
    // Screen UI Root
    // -------------------------------------------------
    auto* root = AddComponent<UIScreenRootComponent>();
    SetRootComponent(root);

    // -------------------------------------------------
    // Root UIScreenTransform (optional but recommended)
    // -------------------------------------------------
    auto* transform = AddComponent<UIScreenTransform>();
    transform->SetRoot(root);
}
