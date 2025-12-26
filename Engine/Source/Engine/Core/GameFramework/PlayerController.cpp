#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/PlayerController.h"
#include "Engine/Core/GameFramework/Pawn.h"
#include "Engine/Core/GameFramework/GameModeBase.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/CameraActor.h"

#include "Engine/Core/Input/InputSystem.h"

// =====================================================
// Constructor / Destructor
// =====================================================

PlayerController::PlayerController(World& world)
    : m_world(world)
{
}

PlayerController::~PlayerController() = default;

// =====================================================
// Lifecycle
// =====================================================

void PlayerController::OnBeginPlay()
{
    // 기본 구현 비워둠
    // GameMode 또는 파생 Controller에서 초기 바인딩 수행
}

void PlayerController::Tick(float deltaTime)
{
    // Pawn이 없거나 파괴 중이면 입력 처리 안 함
    if (!m_pawn || m_pawn->IsPendingDestroy())
        return;

    m_playerInput.BeginFrame(deltaTime);

    // ---------------------------------
    // Input Dispatch
    // ---------------------------------
    InputSystem::Dispatch(m_playerInput);
    ProcessInput(deltaTime);
}

void PlayerController::OnEndPlay()
{
    UnPossess();
    m_camera = nullptr;
}

// =====================================================
// Possession
// =====================================================

void PlayerController::Possess(Pawn* pawn)
{
    if (!CanPossess(pawn))
        return;

    UnPossess();

    m_pawn = pawn;
    pawn->PossessedBy(this);
}

void PlayerController::UnPossess()
{
    if (!m_pawn)
        return;

    m_pawn->UnPossessed();
    m_pawn = nullptr;
}

// =====================================================
// Camera
// =====================================================

void PlayerController::SetCamera(CameraActor* camera)
{
    m_camera = camera;
}

// =====================================================
// Input (override point)
// =====================================================

void PlayerController::ProcessInput(float /*deltaTime*/)
{
    // 파생 클래스에서 구현
    //
    // 예:
    // if (m_playerInput.ConsumePressed(InputAction_RotateCW))
    //     m_pawn->RotateCW();
}

// =====================================================
// Internal
// =====================================================

bool PlayerController::CanPossess(Pawn* pawn) const
{
    if (!pawn)
        return false;

    if (m_world.IsShuttingDown())
        return false;

    if (pawn->IsPendingDestroy())
        return false;

    if (pawn->GetWorld() != &m_world)
        return false;

    if (!pawn->IsPersistentActor())
        return false;

    return true;
}

GameModeBase* PlayerController::GetGameMode() const
{
    return m_world.GetGameMode();
}