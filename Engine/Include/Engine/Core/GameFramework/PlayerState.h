#pragma once

#include "Common/Types.h"

// Forward declarations
class World;

/*
    PlayerState
    -------------------------------------------------
    - 플레이어의 지속 상태를 보관하는 객체
    - Pawn / Level 교체와 무관
    - 멀티플레이 도입 시 핵심 역할
    - 현재는 스켈레톤만 제공 (미사용)
*/
class PlayerState
{
public:
    explicit PlayerState(World& world);
    virtual ~PlayerState();

    PlayerState(const PlayerState&) = delete;
    PlayerState& operator=(const PlayerState&) = delete;

public:
    // =====================================================
    // Accessors
    // =====================================================
    World& GetWorld() const { return m_world; }

protected:
    World& m_world;
};
