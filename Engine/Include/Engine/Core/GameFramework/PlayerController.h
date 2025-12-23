#pragma once

#include "Common/Types.h"

// Forward declarations
class World;
class Actor;
class Pawn;

/*
    PlayerController
    -------------------------------------------------
    - Player input → gameplay logic bridge
    - Pawn을 Possess 하여 조종
*/
class PlayerController
{
public:
    explicit PlayerController(World& world);
    virtual ~PlayerController();

    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    virtual void OnBeginPlay();
    virtual void Tick(float deltaTime);
    virtual void OnEndPlay();

public:
    // =====================================================
    // Possession
    // =====================================================
    void Possess(Pawn* pawn);
    void UnPossess();

    Pawn* GetPawn() const { return m_pawn; }
    World& GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Input
    // =====================================================
    virtual void ProcessInput(float deltaTime);

protected:
    World& m_world;
    Pawn* m_pawn{ nullptr };
};
