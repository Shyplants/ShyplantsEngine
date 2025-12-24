#pragma once

#include "Common/Types.h"

// Forward declarations
class World;
class Pawn;
class CameraActor;
class PlayerState;

/*
    PlayerController
    -------------------------------------------------
    - Player input → gameplay logic bridge
    - Persistent Pawn을 Possess 하여 조종
    - CameraActor와 약한 연결을 가짐 (소유하지 않음)
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
    // Possession (Pawn)
    // =====================================================
    void Possess(Pawn* pawn);
    void UnPossess();

    Pawn* GetPawn() const { return m_pawn; }
    bool  HasPawn() const { return m_pawn != nullptr; }

public:
    // =====================================================
    // Camera binding
    // =====================================================
    void SetCamera(CameraActor* camera);
    CameraActor* GetCamera() const { return m_camera; }

public:
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

protected:
    PlayerState* m_playerState{ nullptr };

private:
    World& m_world;

    Pawn* m_pawn{ nullptr };     // non-owning
    CameraActor* m_camera{ nullptr };   // non-owning
};
