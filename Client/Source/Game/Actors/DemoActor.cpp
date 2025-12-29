#include "ClientPCH/ClientPCH.h"
#include "Game/Actors/DemoActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Core/Component/TransformComponent.h"

#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/TextureResource.h"

DemoActor::DemoActor()
    : WorldActor()
{
}

void DemoActor::OnSpawned()
{
    WorldActor::OnSpawned();

    // --------------------------------------------------
    // SpriteRendererComponent
    // --------------------------------------------------
    m_spriteRenderer = AddComponent<SpriteRendererComponent>();
    SP_ASSERT(m_spriteRenderer != nullptr);

    auto& resourceManager = ResourceManager::Get();

    auto texture = resourceManager.Get<TextureResource>(L"../Resources/test.png");

    m_spriteRenderer->SetTexture(0, texture.get());

    //m_spriteRenderer->SetRenderOffset({ -400.0f, 200.0f });
    //m_spriteRenderer->SetPivot(SpritePivot::TopRight);
}

void DemoActor::OnBeginPlay()
{
    WorldActor::OnBeginPlay();
    // Gameplay logic only
}
