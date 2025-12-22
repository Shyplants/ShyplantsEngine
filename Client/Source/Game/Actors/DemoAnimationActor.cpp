#include "ClientPCH/ClientPCH.h"
#include "Game/Actors/DemoAnimationActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Core/Component/SpriteAnimatorComponent.h"
#include "Engine/Graphics/Animation/SpriteAnimationClip.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"
#include "Engine/Graphics/Material/Material.h"
#include "Engine/Graphics/Material/MaterialManager.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/Engine.h"
#include "Engine/Graphics/Renderer/Renderer.h"

#include "Common/Math/Rect.h"


DemoAnimationActor::DemoAnimationActor()
	: Actor()
{
}

void DemoAnimationActor::OnSpawned()
{
    Actor::OnSpawned();

    m_sprite = AddComponent<SpriteRendererComponent>();
    SP_ASSERT(m_sprite != nullptr);

   
    auto texture = ResourceManager::Get()
        .Load<Texture>(L"../Resources/Sword_Idle_without_shadow.png", TextureColorSpace::SRGB);

    SP_ASSERT(texture != nullptr);

    m_sprite->SetTexture(texture.get());

	m_animator = AddComponent<SpriteAnimatorComponent>();

	SpriteAnimationClip idle;
	idle.name = L"Idle";
	idle.fps = 12;
	idle.loop = true;

	for (int i = 0; i < 12; i++)
	{
		Rect rc = { 64 * i, 0, 64 * (i + 1), 64 };
		idle.frames.push_back(rc);
	}

	m_animator->AddClip(idle);
	m_animator->Play(L"Idle");
}

void DemoAnimationActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);
}
