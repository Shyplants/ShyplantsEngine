#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/GameMode.h"
#include "Engine/Core/GameFramework/GameState.h"
#include "Engine/Core/World/World.h"

// =====================================================
// Constructor / Destructor
// =====================================================

GameMode::GameMode(World& world)
    : m_world(world)
{
    m_gameState = CreateGameState();
}

GameMode::~GameMode()
{
    // Safety: ensure EndPlay is called if needed
    if (m_hasBegunPlay)
    {
        OnEndPlay();
    }
}

// =====================================================
// Lifecycle
// =====================================================

void GameMode::OnBeginPlay()
{
    if (m_hasBegunPlay)
        return;

    m_hasBegunPlay = true;

    if (m_gameState)
    {
        m_gameState->OnBeginPlay();
    }
}

void GameMode::Tick(float deltaTime)
{
    if (!m_hasBegunPlay)
        return;

    if (m_gameState)
    {
        m_gameState->Tick(deltaTime);
    }
}

void GameMode::OnEndPlay()
{
    if (!m_hasBegunPlay)
        return;

    if (m_gameState)
    {
        m_gameState->OnEndPlay();
    }

    m_hasBegunPlay = false;
}

// =====================================================
// Factory
// =====================================================

std::unique_ptr<GameState> GameMode::CreateGameState()
{
    return std::make_unique<GameState>(m_world);
}
