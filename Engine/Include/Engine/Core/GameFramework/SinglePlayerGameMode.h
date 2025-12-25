#pragma once

#include "Engine/Core/GameFramework/GameModeBase.h"

// Forward declarations
class Pawn;

/*
    SinglePlayerGameMode
    -------------------------------------------------
    - 싱글플레이 전용 GameMode
    - PlayerController 1 + Pawn 1 전제
*/
class SinglePlayerGameMode : public GameModeBase
{
public:
    explicit SinglePlayerGameMode(World& world);
    ~SinglePlayerGameMode() override;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    void OnBeginPlay() override;
    void OnEndPlay() override;

public:
    Pawn* GetPawn() const { return m_pawn; }

protected:
    // =====================================================
    // Hooks for derived game modes
    // =====================================================
    virtual Pawn* SpawnDefaultPawn();

protected:
    Pawn* m_pawn{ nullptr }; // non-owning
};
