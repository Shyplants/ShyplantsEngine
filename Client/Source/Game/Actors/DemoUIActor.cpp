#include "ClientPCH/ClientPCH.h"
#include "Game/Actors/DemoUIActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Core/Component/TransformComponent.h"
#include "Engine/UI/Component/UITransformComponent.h"

#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/TextureResource.h"

DemoUIActor::DemoUIActor() = default;

DemoUIActor::~DemoUIActor() = default;

void DemoUIActor::OnSpawned()
{
	UIActor::OnSpawned();

    auto* uiRoot = GetRootUITransform();
    SP_ASSERT(uiRoot);

    UIAnchorData anchor{};
    anchor.Anchor = EUIAnchor::Center;

    uiRoot->SetAnchor(anchor);


    // --------------------------------------------------
    // SpriteRendererComponent
    // --------------------------------------------------
    m_spriteRenderer = AddComponent<SpriteRendererComponent>();
    SP_ASSERT(m_spriteRenderer != nullptr);

    auto& resourceManager = ResourceManager::Get();
    auto texture = resourceManager.Get<TextureResource>(L"../Resources/test.png");

    m_spriteRenderer->SetTexture(0, texture.get());
    m_spriteRenderer->SetRenderCategory({ ERenderSpace::Screen, ERenderLayer::Default });
    // m_spriteRenderer->SetPivot(SpritePivot::BottomLeft);

    // _spriteRenderer->SetRenderOffset({ +400.0f, -200.0f });
    //m_spriteRenderer->SetPivot(SpritePivot::TopRight);
}