#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/PlayerController.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"

#include "Engine/Platform/Input/InputSystem.h"

PlayerController::PlayerController(World& world)
    : m_world(world)
{
}

PlayerController::~PlayerController()
{
    OnEndPlay();
}

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

void PlayerController::Possess(Actor* actor)
{
    SP_ASSERT(actor != nullptr);
    m_pawn = actor;
}

void PlayerController::UnPossess()
{
    m_pawn = nullptr;
}

void PlayerController::ProcessInput(float /*deltaTime*/)
{
    // 기본 구현은 비워둠
    // 파생 클래스에서 오버라이드
}
