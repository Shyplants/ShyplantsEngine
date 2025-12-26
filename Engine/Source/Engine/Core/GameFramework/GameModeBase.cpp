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
}

GameModeBase::~GameModeBase() = default;

// =====================================================
// Initialization
// =====================================================

void GameModeBase::Initialize()
{
    if (m_initialized)
        return;

    m_initialized = true;

    if (!m_gameState)
        m_gameState = CreateGameState();

    if (!m_playerController)
        m_playerController = CreatePlayerController();
}

// =====================================================
// Lifecycle
// =====================================================

void GameModeBase::OnBeginPlay()
{
    if (m_hasBegunPlay)
        return;

    SP_ASSERT(m_initialized && "GameModeBase::Initialize() must be called before OnBeginPlay");

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

void GameModeBase::OnGamePlayStart()
{
}

// =====================================================
// Factory (default implementations)
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
    return nullptr;
}
