#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/WorldTransformComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"

using namespace DirectX;

// =====================================================
// Constructor
// =====================================================

WorldTransformComponent::WorldTransformComponent(Actor* owner)
    : TransformComponent(owner)
{
}

// =====================================================
// Local setters
// =====================================================

void WorldTransformComponent::SetLocalPosition(const XMFLOAT3& pos)
{
    m_localPosition = pos;
    MarkDirtyRecursive();
}

void WorldTransformComponent::SetLocalRotation(const XMFLOAT3& rot)
{
    m_localRotation = rot;
    MarkDirtyRecursive();
}

void WorldTransformComponent::SetLocalScale(const XMFLOAT3& scale)
{
    m_localScale = scale;
    MarkDirtyRecursive();
}

// =====================================================
// World queries
// =====================================================

XMMATRIX WorldTransformComponent::GetWorldMatrix() const
{
    if (!m_worldDirty)
        return m_cachedWorld;

    const XMMATRIX S = XMMatrixScaling(
        m_localScale.x,
        m_localScale.y,
        m_localScale.z);

    const XMMATRIX R = XMMatrixRotationRollPitchYaw(
        XMConvertToRadians(m_localRotation.x),
        XMConvertToRadians(m_localRotation.y),
        XMConvertToRadians(m_localRotation.z));

    const XMMATRIX T = XMMatrixTranslation(
        m_localPosition.x,
        m_localPosition.y,
        m_localPosition.z);

    const XMMATRIX local = S * R * T;

    if (GetParent())
    {
        m_cachedWorld =
            GetParent()->GetWorldMatrix() * local;
    }
    else
    {
        m_cachedWorld = local;
    }

    m_worldDirty = false;
    return m_cachedWorld;
}

XMFLOAT3 WorldTransformComponent::GetWorldPositionFast() const
{
    const XMMATRIX& W = GetWorldMatrix();

    return {
        W.r[3].m128_f32[0],
        W.r[3].m128_f32[1],
        W.r[3].m128_f32[2]
    };
}

// =====================================================
// Camera registration
// =====================================================

void WorldTransformComponent::RegisterCamera(CameraComponent2D* camera)
{
#if defined(_DEBUG)
    SP_ASSERT(camera != nullptr);
#endif

    if (std::find(
        m_cameras.begin(),
        m_cameras.end(),
        camera) == m_cameras.end())
    {
        m_cameras.push_back(camera);
    }
}

void WorldTransformComponent::UnregisterCamera(CameraComponent2D* camera)
{
    m_cameras.erase(
        std::remove(
            m_cameras.begin(),
            m_cameras.end(),
            camera),
        m_cameras.end());
}


void WorldTransformComponent::OnTransformDirty()
{
    TransformComponent::OnTransformDirty();

    for (CameraComponent2D* cam : m_cameras)
    {
        if (cam)
        {
            cam->OnOwnerTransformDirty();
        }
    }
}
