#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Actor.h"

#include "Engine/Core/Component/CameraComponent2D.h"
#include "Engine/Core/Component/WorldTransformComponent.h"

using namespace DirectX;

// =====================================================
// Constructor / Destructor
// =====================================================

CameraComponent2D::CameraComponent2D(Actor* owner)
    : ActorComponent(owner)
{
}

CameraComponent2D::~CameraComponent2D() = default;

// =====================================================
// Lifecycle
// =====================================================

void CameraComponent2D::OnRegister()
{
    ActorComponent::OnRegister();

    m_transform =
        GetOwner()->GetComponent<WorldTransformComponent>();

#if defined(_DEBUG)
    SP_ASSERT(m_transform != nullptr);
#endif

    m_transform->RegisterCamera(this);

    m_viewDirty = true;
    m_viewProjDirty = true;
}

void CameraComponent2D::OnUnregister()
{
    if (m_transform)
    {
        m_transform->UnregisterCamera(this);
        m_transform = nullptr;
    }
}

// =====================================================
// Camera parameters
// =====================================================

void CameraComponent2D::SetViewSize(float width, float height)
{
    SP_ASSERT(width > 0.f && height > 0.f);

    if (m_viewSize.x == width && m_viewSize.y == height)
        return;

    m_viewSize = { width, height };
    m_projDirty = true;
    m_viewProjDirty = true;
}

void CameraComponent2D::SetZoom(float zoom)
{
    SP_ASSERT(zoom > 0.f);

    if (fabsf(m_zoom - zoom) < 0.0001f)
        return;

    m_zoom = zoom;
    m_projDirty = true;
    m_viewProjDirty = true;
}

void CameraComponent2D::SetNearFar(float nearZ, float farZ)
{
    SP_ASSERT(nearZ < farZ);

    if (m_nearZ == nearZ && m_farZ == farZ)
        return;

    m_nearZ = nearZ;
    m_farZ = farZ;
    m_projDirty = true;
    m_viewProjDirty = true;
}

// =====================================================
// Transform Dirty notification
// =====================================================

void CameraComponent2D::OnOwnerTransformDirty()
{
    m_viewDirty = true;
    m_viewProjDirty = true;
}

// =====================================================
// Matrices
// =====================================================

const XMMATRIX& CameraComponent2D::GetViewMatrix() const
{
    if (m_viewDirty)
        RecalculateView();

    return m_view;
}

const XMMATRIX& CameraComponent2D::GetProjectionMatrix() const
{
    SP_ASSERT(IsViewSizeValid());

    if (m_projDirty)
        RecalculateProjection();

    return m_projection;
}

const XMMATRIX& CameraComponent2D::GetViewProjectionMatrix() const
{
    SP_ASSERT(IsViewSizeValid());

    if (m_viewProjDirty)
        RecalculateViewProjection();

    return m_viewProjection;
}

// =====================================================
// Internal calculations
// =====================================================

void CameraComponent2D::RecalculateView() const
{
    const XMFLOAT3 pos =
        m_transform->GetWorldPositionFast();

    m_view = XMMatrixTranslation(
        -pos.x,
        -pos.y,
        0.f);

    m_viewDirty = false;
}

void CameraComponent2D::RecalculateProjection() const
{
    const float halfW = (m_viewSize.x * 0.5f) / m_zoom;
    const float halfH = (m_viewSize.y * 0.5f) / m_zoom;

    m_projection = XMMatrixOrthographicOffCenterLH(
        -halfW, halfW,
        -halfH, halfH,
        m_nearZ,
        m_farZ);

    m_projDirty = false;
}

void CameraComponent2D::RecalculateViewProjection() const
{
    if (m_viewDirty)
        RecalculateView();

    if (m_projDirty)
        RecalculateProjection();

    m_viewProjection =
        XMMatrixMultiply(m_view, m_projection);

    m_viewProjDirty = false;
}
