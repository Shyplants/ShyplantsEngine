#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/PlayerController.h"
#include "Engine/Core/GameFramework/Pawn.h"
#include "Engine/Core/World/World.h"

PlayerController::PlayerController(World& world)
    : m_world(world)
{
}

PlayerController::~PlayerController() = default;

void PlayerController::OnBeginPlay()
{
}

void PlayerController::Tick(float deltaTime)
{
    ProcessInput(deltaTime);
}

void PlayerController::OnEndPlay()
{
    UnPossess();
}

void PlayerController::Possess(Pawn* pawn)
{
    SP_ASSERT(pawn != nullptr);

    UnPossess();

    m_pawn = pawn;
    pawn->PossessedBy(this);
}

void PlayerController::UnPossess()
{
    if (m_pawn)
    {
        m_pawn->UnPossessed();
        m_pawn = nullptr;
    }
}

void PlayerController::ProcessInput(float /*deltaTime*/)
{
    // 파생 Controller에서 구현
}
