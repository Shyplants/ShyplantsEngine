#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/CameraActor.h"
#include "Engine/Core/World/World.h"

#include "Engine/Core/Component/CameraComponent2D.h"

// =====================================================
// Constructor / Destructor
// =====================================================

CameraActor::CameraActor() = default;

CameraActor::~CameraActor() = default;

// =====================================================
// Actor Overrides
// =====================================================

void CameraActor::OnSpawned()
{
    // CameraComponent 생성
    m_camera = AddComponent<CameraComponent2D>();
    SP_ASSERT(m_camera != nullptr);

    // World에 ActiveCamera로 등록
    World* world = GetWorld();
    SP_ASSERT(world != nullptr);

    world->SetActiveCamera(m_camera);
}
