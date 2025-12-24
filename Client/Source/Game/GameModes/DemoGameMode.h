#pragma once

#include "Engine/Core/GameFramework/SinglePlayerGameMode.h"

// Forward declarations
class CameraActor;
class Pawn;

/*
    DemoGameMode
    -------------------------------------------------
    - 데모/싱글플레이용 게임 규칙
    - Persistent 영역 초기 세팅 담당
*/
class DemoGameMode : public SinglePlayerGameMode
{
public:
    explicit DemoGameMode(World& world);
    ~DemoGameMode() override;

protected:
    // =====================================================
    // SinglePlayerGameMode overrides
    // =====================================================
    void OnBeginPlay() override;

protected:
    // =====================================================
    // Pawn policy
    // =====================================================
    Pawn* SpawnDefaultPawn() override;

protected:
    // =====================================================
    // Internal helpers
    // =====================================================
    void SpawnCamera();

private:
    CameraActor* m_camera{ nullptr }; // non-owning
};
