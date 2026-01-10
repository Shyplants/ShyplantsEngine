#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/PersistentLevel.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"

// =====================================================
// Constructor / Destructor
// =====================================================

PersistentLevel::PersistentLevel() = default;
PersistentLevel::~PersistentLevel() = default;

// =====================================================
// Lifecycle
// =====================================================

void PersistentLevel::OnEnter(World& world)
{
    Level::OnEnter(world);
}

void PersistentLevel::OnExit(World& world)
{
    Level::OnExit(world);
}

void PersistentLevel::OnBeginPlay()
{
    Level::OnBeginPlay();
}

void PersistentLevel::Shutdown()
{
    Level::Shutdown();
}

// =====================================================
// Tick
// =====================================================

void PersistentLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);
}
