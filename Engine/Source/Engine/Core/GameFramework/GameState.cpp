#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/GameState.h"
#include "Engine/Core/World/World.h"

// =====================================================
// Constructor
// =====================================================

GameState::GameState(World* world)
    : m_world(world)
{
    SP_ASSERT(m_world != nullptr);
}

GameState::~GameState() = default;

// =====================================================
// Lifecycle
// =====================================================

void GameState::OnBeginPlay()
{
    if (m_hasBegunPlay)
        return;

    m_hasBegunPlay = true;
}

void GameState::Tick(float /*deltaTime*/)
{
    if (!m_hasBegunPlay)
        return;
}

void GameState::OnEndPlay()
{
    if (!m_hasBegunPlay)
        return;

    m_hasBegunPlay = false;
}
