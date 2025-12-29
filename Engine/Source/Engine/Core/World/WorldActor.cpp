#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/WorldActor.h"
#include "Engine/Core/Component/WorldTransformComponent.h"

// =====================================================
// Constructor / Destructor
// =====================================================

WorldActor::WorldActor() = default;
WorldActor::~WorldActor() = default;

// =====================================================
// Lifecycle
// =====================================================

void WorldActor::OnSpawned()
{
    Actor::OnSpawned();

    // -------------------------------------------------
    // Root World Transform
    // -------------------------------------------------
    m_worldTransform = AddComponent<WorldTransformComponent>();
    SetRootTransform(m_worldTransform);
}
