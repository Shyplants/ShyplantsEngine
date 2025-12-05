#include "ClientPCH/ClientPCH.h"
#include "Actors/DemoActor.h"

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"
#include "Engine/Core/Component/SceneComponent.h"


// Text Test
#include "Engine/Core/Engine.h"
#include "Engine/Core/Component/TextRendererComponent.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"


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

    auto text = AddComponent<TextRendererComponent>(this);

    auto renderer = Engine::Get().GetRenderer();
    text->SetFont(renderer->GetDefaultFont());
    text->SetText(L"Hello Shyplants! 123");
    text->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
    text->SetScale(1.0f);
}

void DemoActor::BeginPlay()
{
    
}
