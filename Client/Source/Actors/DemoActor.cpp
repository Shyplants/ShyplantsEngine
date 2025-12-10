#include "ClientPCH/ClientPCH.h"
#include "Actors/DemoActor.h"

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"
#include "Engine/Core/Component/SceneComponent.h"

DemoActor::DemoActor()
    : Actor()
{
}

DemoActor::~DemoActor()
{
}

void DemoActor::OnSpawned()
{
    auto tex = ResourceManager::Get().Load<Texture>(L"../Resources/test.png").get();

    m_sprite = AddComponent<SpriteRendererComponent>(this);
    m_sprite->SetTexture(tex);
    // m_sprite->SetRenderOffset({ 200.0f, 200.0f });
}

void DemoActor::BeginPlay()
{
    
}
