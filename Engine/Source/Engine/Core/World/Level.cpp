#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"
// #include "Engine/Core/Component/CameraComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/World/World.h"

#include "SpriteBatch.h"

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

void Level::Render(D3D11Renderer& renderer)
{
	// WORLD PASS
	RenderWorldObjects(renderer);

	// UI PASS (항상 화면 최상위)
	RenderUI(renderer);
}

Actor* Level::SpawnActorInternal(std::unique_ptr<Actor> actor)
{
	Actor* rawPtr = actor.get();

	rawPtr->SetWorld(m_world);
	rawPtr->OnSpawned();
	m_actors.push_back(std::move(actor));

	if (m_hasBegunPlay)
		rawPtr->BeginPlay();

	
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

void Level::RenderWorldObjects(D3D11Renderer& renderer)
{
	CameraComponent2D* camera = m_world->GetMainCamera();
	if (!camera)
		return;

	auto viewProj = camera->GetViewProjMatrix();

	for (auto& actor : m_actors)
	{
		if (actor->IsPendingDestroy())
			continue;

		const auto& comps = actor->GetComponents();

		for (const auto& comp : comps)
		{
			if (auto rc = dynamic_cast<RendererComponent*>(comp.get()))
			{
				rc->RenderWorld(renderer, viewProj);
			}
		}
	}
}

void Level::RenderUI(D3D11Renderer& renderer)
{
	auto spriteBatch = renderer.GetSpriteBatch();
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);

	for (auto& actor : m_actors)
	{
		if (actor->IsPendingDestroy())
			continue;

		const auto& comps = actor->GetComponents();

		for (const auto& comp : comps)
		{
			if (auto rc = dynamic_cast<RendererComponent*>(comp.get()))
			{
				rc->RenderUI(renderer);
			}
		}
	}

	spriteBatch->End();
}
