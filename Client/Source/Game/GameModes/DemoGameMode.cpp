#include "ClientPCH/ClientPCH.h"

#include "Game/GameModes/DemoGameMode.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/CameraActor.h"
#include "Engine/Core/GameFramework/Pawn.h"
#include "Engine/Core/GameFramework/PlayerController.h"

#include "Engine/Core/Component/TransformComponent.h"

#include "Game/Actors/DemoActor.h"

// =====================================================
// Constructor / 
// =====================================================

DemoGameMode::DemoGameMode(World& world)
    : SinglePlayerGameMode(world)
{
}

DemoGameMode::~DemoGameMode() = default;

// =====================================================
// Pawn
// =====================================================

Pawn* DemoGameMode::SpawnDefaultPawn()
{
    World& world = GetWorld();

    // Persistent Pawn 스폰
    Pawn* pawn = world.SpawnActor<Pawn>();
    SP_ASSERT(pawn != nullptr);

    return pawn;
}

// =====================================================
// Camera
// =====================================================

void DemoGameMode::SpawnCamera()
{
    World& world = GetWorld();

    m_camera = world.SpawnActor<CameraActor>();
    SP_ASSERT(m_camera != nullptr);
}

// =====================================================
// Lifecycle
// =====================================================

void DemoGameMode::OnBeginPlay()
{
    // -------------------------------------------------
    // 1. Base 초기화 (Pawn 생성 + Possess)
    // -------------------------------------------------
    SinglePlayerGameMode::OnBeginPlay();

    // -------------------------------------------------
    // 2. Camera 스폰
    // -------------------------------------------------
    SpawnCamera();

    // -------------------------------------------------
    // 3. 게임 시작 상태 로그
    // -------------------------------------------------
    SP_LOG(LogGame, ELogLevel::Info,
        "DemoGameMode initialized");


    auto& world = GetWorld();

    auto* demoActor = world.SpawnActor<DemoActor>();
    // demoActor->GetRootTransform()->SetLocalPosition({ -300.0f, 0.0f, 20.0f });

    auto* demoActor2 = world.SpawnActor<DemoActor>();
    // demoActor2->GetRootTransform()->SetLocalPosition({ 0.0f, 0.0f, 11.0f });
}
