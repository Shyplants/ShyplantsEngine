#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/Pawn.h"
#include "Engine/Core/GameFramework/PlayerController.h"

Pawn::Pawn() = default;

Pawn::~Pawn() = default;

void Pawn::PossessedBy(PlayerController* controller)
{
    SP_ASSERT(controller != nullptr);

    if (m_controller == controller)
        return;

    if (m_controller)
    {
        UnPossessed();
    }

    m_controller = controller;
    OnPossessed();
}

void Pawn::UnPossessed()
{
    if (!m_controller)
        return;

    OnUnPossessed();
    m_controller = nullptr;
}

void Pawn::OnPossessed()
{
    // 기본 구현 비워둠
}

void Pawn::OnUnPossessed()
{
}
