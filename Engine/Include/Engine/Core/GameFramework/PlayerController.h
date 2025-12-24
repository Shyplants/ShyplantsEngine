#pragma once

#include "Common/Types.h"

// Forward declarations
class World;
class Pawn;

/*
    PlayerController
    -------------------------------------------------
    - Player input → gameplay logic bridge
    - Persistent Pawn을 Possess 하여 조종
    - Level 교체와 무관한 World 소속 객체
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
    bool  HasPawn() const { return m_pawn != nullptr; }

    World& GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Input
    // =====================================================
    virtual void ProcessInput(float deltaTime);

private:
    // =====================================================
    // Internal
    // =====================================================
    bool CanPossess(Pawn* pawn) const;

private:
    World& m_world;
    Pawn* m_pawn{ nullptr };
};
