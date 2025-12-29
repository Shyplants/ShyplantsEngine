#pragma once

#include "Engine/Core/World/Actor.h"

// Forward declarations
class PlayerController;

/*
    Pawn
    -------------------------------------------------
    - PlayerController에 의해 조종되는 Actor
    - 입력은 Controller, 물리/행동은 Pawn
*/
class Pawn : public Actor
{
public:
    Pawn();
    virtual ~Pawn() override;

    // -------------------------------------------------
    // Level Ownership
    // -------------------------------------------------
    bool IsPersistentActor() const override { return true; }
    bool IsUIActor() const override { return false; }

    // -------------------------------------------------
    // Possession
    // -------------------------------------------------
    virtual void PossessedBy(PlayerController* controller);
    virtual void UnPossessed();

    PlayerController* GetController() const { return m_controller; }
    bool IsPossessed() const { return m_controller != nullptr; }

protected:
    virtual void OnPossessed();
    virtual void OnUnPossessed();

protected:
    PlayerController* m_controller{ nullptr };
};
