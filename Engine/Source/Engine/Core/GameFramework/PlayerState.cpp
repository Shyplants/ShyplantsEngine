#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/GameFramework/PlayerState.h"
#include "Engine/Core/World/World.h"

// =====================================================
// Constructor / Destructor
// =====================================================

PlayerState::PlayerState(World& world)
    : m_world(world)
{
}

PlayerState::~PlayerState() = default;
