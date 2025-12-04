#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Level.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/RendererComponent.h"
#include "Engine/Core/Component/CameraComponent.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"


World::World()
{

}

World::~World()
{
	UnloadCurrentLevel();
}

void World::Tick(float deltaTime)
{
	if (m_currentLevel)
		m_currentLevel->Tick(deltaTime);
}

void World::Render(D3D11Renderer& renderer)
{
	if (!m_currentLevel)
		return;

	m_currentLevel->Render(renderer, m_activeCamera);
}

void World::DestroyActor(Actor* actor)
{
	if (m_currentLevel)
		m_currentLevel->MarkActorForDestroy(actor);
}

void World::LoadLevel(std::unique_ptr<Level> level)
{
	UnloadCurrentLevel();

	m_currentLevel = std::move(level);

	if (m_currentLevel)
		m_currentLevel->OnBeginPlay();
}

void World::UnloadCurrentLevel()
{
	if (m_currentLevel)
	{
		m_currentLevel.reset();
	}
}

Actor* World::SpawnActor_Impl(std::unique_ptr<Actor> actor)
{
	if (!m_currentLevel)
		return nullptr;

	return m_currentLevel->SpawnActorInternal(std::move(actor));
}
