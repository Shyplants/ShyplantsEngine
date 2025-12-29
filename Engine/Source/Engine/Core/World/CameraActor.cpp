#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/CameraActor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/CameraComponent2D.h"

CameraActor::CameraActor() = default;

void CameraActor::OnSpawned()
{
    WorldActor::OnSpawned();

    m_camera = AddComponent<CameraComponent2D>();
    SP_ASSERT(m_camera != nullptr);

    World* world = GetWorld();
    SP_ASSERT(world != nullptr);

    world->SetActiveCamera(m_camera);
}
