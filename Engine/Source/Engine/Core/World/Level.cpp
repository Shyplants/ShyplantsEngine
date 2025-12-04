#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Core/Component/CameraComponent.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/World/World.h"

Level::Level(World* world)
	: m_world(world)
{
}

Level::~Level()
{
	m_actors.clear();
}

void Level::OnBeginPlay()
{
	m_hasBegunPlay = true;

	for (auto& actor : m_actors)
	{
		actor->BeginPlay();
	}

}

void Level::Tick(float deltaTime)
{
	// 스폰된 Actor 중 제거 대상이 있으면 정리
	ProcessDestroyedActors();

	for (auto& actor : m_actors)
	{
		actor->Tick(deltaTime);
	}
}

void Level::Render(D3D11Renderer& renderer, const CameraComponent* camera)
{
	if (!camera)
		return;

	const DirectX::XMMATRIX viewProj = camera->GetViewProjMatrix();

	for (auto& actor : m_actors)
	{
		if (actor->IsPendingDestroy())
			continue;

		const auto& comps = actor->GetComponents();

		for (const auto& comp : comps)
		{
			// RendererComponent만 DrawCommand 제출
			if (auto rc = dynamic_cast<RendererComponent*>(comp.get()))
			{
				rc->Render(renderer, viewProj);
			}
		}
	}
}

Actor* Level::SpawnActorInternal(std::unique_ptr<Actor> actor)
{
	Actor* rawPtr = actor.get();

	rawPtr->SetWorld(m_world);

	rawPtr->OnSpawned();

	if (m_hasBegunPlay)
		rawPtr->BeginPlay();

	m_actors.push_back(std::move(actor));

	return rawPtr;
}

void Level::MarkActorForDestroy(Actor* actor)
{
	if (!actor)
		return;

	m_destroyQueue.push_back(actor);
}

void Level::ProcessDestroyedActors()
{
	if (m_destroyQueue.empty())
		return;

	m_actors.erase(
		std::remove_if(
			m_actors.begin(), m_actors.end(),
			[&](std::unique_ptr<Actor>& actor)
			{
				return actor->IsPendingDestroy();
			}),
		m_actors.end()
	);

	m_destroyQueue.clear();
}