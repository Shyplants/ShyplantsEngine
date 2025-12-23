#pragma once

#include "Common/Types.h"

// Forward declarations
class World;
class Actor;

/*
    PlayerController
    -------------------------------------------------
    - Player input ¡æ gameplay logic bridge
    - Owns NO actors
    - Controls (possesses) one Actor
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
    void Possess(Actor* actor);
    void UnPossess();

    Actor* GetPawn() const { return m_pawn; }
    World& GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Input hooks
    // =====================================================
    virtual void ProcessInput(float deltaTime);

protected:
    World& m_world;
    Actor* m_pawn{ nullptr };
};
