#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/SinglePlayerGameMode.h"
#include "Engine/Core/GameFramework/PlayerController.h"
#include "Engine/Core/GameFramework/Pawn.h"
#include "Engine/Core/World/World.h"

// =====================================================
// Constructor / Destructor
// =====================================================

SinglePlayerGameMode::SinglePlayerGameMode(World& world)
    : GameModeBase(world)
{
}

SinglePlayerGameMode::~SinglePlayerGameMode() = default;

// =====================================================
// Lifecycle
// =====================================================

void SinglePlayerGameMode::OnBeginPlay()
{
    GameModeBase::OnBeginPlay();

    // -----------------------------
    // Spawn & Possess Pawn
    // -----------------------------
    m_pawn = SpawnDefaultPawn();

    if (m_pawn && GetPlayerController())
    {
        GetPlayerController()->Possess(m_pawn);
    }
}

void SinglePlayerGameMode::OnEndPlay()
{
    if (GetPlayerController())
    {
        GetPlayerController()->UnPossess();
    }

    m_pawn = nullptr;

    GameModeBase::OnEndPlay();
}

// =====================================================
// Pawn Spawn
// =====================================================

Pawn* SinglePlayerGameMode::SpawnDefaultPawn()
{
    return GetWorld().SpawnActor<Pawn>();
}
