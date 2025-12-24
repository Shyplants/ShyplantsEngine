#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/PlayerController.h"
#include "Engine/Core/GameFramework/Pawn.h"
#include "Engine/Core/World/World.h"

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
}

void PlayerController::Tick(float deltaTime)
{
    // Pawn이 없거나, 파괴 예정이면 입력 처리 금지
    if (!m_pawn || m_pawn->IsPendingDestroy())
        return;

    ProcessInput(deltaTime);
}

void PlayerController::OnEndPlay()
{
    UnPossess();
}

// =====================================================
// Possession
// =====================================================

void PlayerController::Possess(Pawn* pawn)
{
    if (!CanPossess(pawn))
        return;

    // 기존 Pawn 해제
    UnPossess();

    m_pawn = pawn;
    pawn->PossessedBy(this);
}

void PlayerController::UnPossess()
{
    if (!m_pawn)
        return;

    // Pawn 쪽에서 Controller를 정리
    m_pawn->UnPossessed();
    m_pawn = nullptr;
}

// =====================================================
// Input
// =====================================================

void PlayerController::ProcessInput(float /*deltaTime*/)
{
    // 파생 Controller에서 구현
}

// =====================================================
// Internal
// =====================================================

bool PlayerController::CanPossess(Pawn* pawn) const
{
    if (!pawn)
        return false;

    // World 종료 중이면 조종 금지
    if (m_world.IsShuttingDown())
        return false;

    // 이미 파괴 중인 Pawn은 조종 금지
    if (pawn->IsPendingDestroy())
        return false;

    // 다른 World의 Pawn 조종 금지
    if (pawn->GetWorld() != &m_world)
        return false;

    // 엔진 규칙: PlayerController는 Persistent Pawn만 조종
    if (!pawn->IsPersistentActor())
        return false;

    return true;
}
