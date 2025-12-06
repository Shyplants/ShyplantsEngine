#include "ClientPCH/ClientPCH.h"
#include "Actors/DemoAnimationActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Core/Component/SpriteAnimatorComponent.h"
#include "Engine/Graphics/Animation/SpriteAnimationClip.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"
#include "Engine/Core/Component/SceneComponent.h"


DemoAnimationActor::DemoAnimationActor()
	: Actor()
{
}

DemoAnimationActor::~DemoAnimationActor()
{
}

void DemoAnimationActor::OnSpawned()
{
	m_renderer = AddComponent<SpriteRendererComponent>(this);
	m_renderer->SetTexture(ResourceManager::Get().Load<Texture>(L"../Resources/Sword_Idle_without_shadow.png").get());

	m_animator = AddComponent<SpriteAnimatorComponent>(this);

	SpriteAnimationClip idle;
	idle.name = L"Idle";
	idle.fps = 12;
	idle.loop = true;

	for (int i = 0; i < 12; i++)
	{
		RECT rc = { 64 * i, 0, 64 * (i + 1), 64 };
		idle.frames.push_back(rc);
	}

	m_animator->AddClip(idle);
	m_animator->Play(L"Idle");
}

void DemoAnimationActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);
}
