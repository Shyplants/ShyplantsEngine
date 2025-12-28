#pragma once

#include "Engine/Core/World/Level.h"

/*
    PersistentLevel
    -------------------------------------------------
    - World 수명 동안 유지되는 Level
    - UI(Screen Space), Camera, Debug Actor 소속
    - GameplayLevel 교체와 무관
*/
class PersistentLevel final : public Level
{
public:
    PersistentLevel();
    ~PersistentLevel() override;

    PersistentLevel(const PersistentLevel&) = delete;
    PersistentLevel& operator=(const PersistentLevel&) = delete;

public:
    // =====================================================
    // Level lifecycle
    // =====================================================
    void OnEnter(World& world) override;
    void OnExit(World& world) override;
    void OnBeginPlay() override;
    void Shutdown() override;

    ELevelType GetLevelType() const override
    {
        return ELevelType::Persistent;
    }

public:
    // =====================================================
    // Tick
    // =====================================================
    void Tick(float deltaTime) override;
};
