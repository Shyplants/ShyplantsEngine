#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/GameFramework/GameState.h"
#include "Engine/Core/World/World.h"

GameState::GameState(World* world)
	: m_world(world)
{
}

GameState::~GameState()
{
}

void GameState::Tick(float deltaTime)
{
	m_elapsedTime += deltaTime;
}