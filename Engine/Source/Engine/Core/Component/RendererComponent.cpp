#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"
#include "Engine/Core/World/Actor.h"

// =====================================================
// Constructor
// =====================================================

RendererComponent::RendererComponent(Actor* owner)
    : ActorComponent(owner)
{
}

// =====================================================
// Lifecycle
// =====================================================

void RendererComponent::OnRegister()
{
    ActorComponent::OnRegister();
}

void RendererComponent::Submit(RenderQueue& queue, CameraComponent2D* activeCamera)
{
    if (!m_visible)
        return;

    if (m_category.Space == ERenderSpace::Screen)
    {
        SubmitScreen(queue);
    }
    else
    {
        if (!activeCamera)
            return;

        SubmitWorld(
            queue,
            activeCamera->GetViewProjectionMatrix());
    }
}
