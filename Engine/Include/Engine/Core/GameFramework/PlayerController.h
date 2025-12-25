#pragma once

#include "Common/Types.h"
#include "Engine/Core/Input/PlayerInput.h"

// Forward declarations
class World;
class Pawn;
class CameraActor;
class PlayerState;

/*
    PlayerController
    -------------------------------------------------
    - PlayerInput을 소유
    - InputSystem → PlayerInput → Pawn/Camera로 전달
    - 멀티플레이/리플레이 대응 가능한 구조
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

public:
    // =====================================================
    // Camera
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

protected:
    PlayerInput& GetPlayerInput() { return m_playerInput; }

private:
    bool CanPossess(Pawn* pawn) const;

protected:
    PlayerState* m_playerState{ nullptr };

private:
    World& m_world;
    PlayerInput  m_playerInput;

    Pawn* m_pawn{ nullptr };     // non-owning
    CameraActor* m_camera{ nullptr };   // non-owning
};
