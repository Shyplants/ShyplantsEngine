#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/GameFramework/GameMode.h"
#include "Engine/Core/GameFramework/GameState.h"
#include "Engine/Core/World/World.h"

GameMode::GameMode(World* world)
	: m_world(world)
{
	m_gameState = CreateGameState();
}

GameMode::~GameMode()
{
}

void GameMode::OnBeginPlay()
{
	if (m_gameState)
		m_gameState->OnBeginPlay();
}

void GameMode::Tick(float deltaTime)
{
	if (m_gameState)
		m_gameState->Tick(deltaTime);
}

std::unique_ptr<GameState> GameMode::CreateGameState()
{
	return std::make_unique<GameState>(m_world);
}
