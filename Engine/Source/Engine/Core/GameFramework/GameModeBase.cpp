#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/GameModeBase.h"
#include "Engine/Core/GameFramework/GameState.h"
#include "Engine/Core/GameFramework/PlayerController.h"
#include "Engine/Core/GameFramework/PlayerState.h"
#include "Engine/Core/World/World.h"

// =====================================================
// Constructor / Destructor
// =====================================================

GameModeBase::GameModeBase(World& world)
    : m_world(world)
{
    m_gameState = CreateGameState();
    m_playerController = CreatePlayerController();
}

GameModeBase::~GameModeBase() = default;

// =====================================================
// Lifecycle
// =====================================================

void GameModeBase::OnBeginPlay()
{
    if (m_hasBegunPlay)
        return;

    m_hasBegunPlay = true;

    if (m_gameState)
        m_gameState->OnBeginPlay();

    if (m_playerController)
        m_playerController->OnBeginPlay();
}

void GameModeBase::Tick(float deltaTime)
{
    if (!m_hasBegunPlay)
        return;

    if (m_playerController)
        m_playerController->Tick(deltaTime);

    if (m_gameState)
        m_gameState->Tick(deltaTime);
}

void GameModeBase::OnEndPlay()
{
    if (!m_hasBegunPlay)
        return;

    if (m_playerController)
        m_playerController->OnEndPlay();

    if (m_gameState)
        m_gameState->OnEndPlay();

    m_hasBegunPlay = false;
}

// =====================================================
// Factory
// =====================================================

std::unique_ptr<GameState> GameModeBase::CreateGameState()
{
    return std::make_unique<GameState>(m_world);
}

std::unique_ptr<PlayerController> GameModeBase::CreatePlayerController()
{
    return std::make_unique<PlayerController>(m_world);
}

std::unique_ptr<PlayerState> GameModeBase::CreatePlayerState()
{
    // TODO
    return std::unique_ptr<PlayerState>();
}
