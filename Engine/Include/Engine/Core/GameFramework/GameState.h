#pragma once

#include "Common/Types.h"

/*
    GameState
    -------------------------------------------------
    - Gameplay 상태 데이터 컨테이너
    - GameMode가 제어하는 대상
    - World / Level / Render와 직접 결합하지 않음
*/
class GameState
{
public:
    explicit GameState(class World* world);
    virtual ~GameState();

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================

    // Called once when gameplay session begins
    virtual void OnBeginPlay();

    // Called every frame during gameplay session
    virtual void Tick(float deltaTime);

    // Called once when gameplay session ends
    virtual void OnEndPlay();

public:
    // =====================================================
    // Accessors
    // =====================================================
    class World* GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Internal helpers (for derived states)
    // =====================================================
    bool HasBegunPlay() const { return m_hasBegunPlay; }

protected:
    class World* m_world{ nullptr };

private:
    bool m_hasBegunPlay{ false };
};
