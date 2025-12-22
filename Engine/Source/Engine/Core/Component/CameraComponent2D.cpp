#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/CameraComponent2D.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;

// =========================================================
// Constructor
// =========================================================

CameraComponent2D::CameraComponent2D(Actor* owner)
    : SceneComponent(owner)
{
    // ViewSize는 반드시 외부에서 SetViewSize 호출로 주입되어야 함
}

// =========================================================
// View Size
// =========================================================

void CameraComponent2D::SetViewSize(float width, float height)
{
    SP_ASSERT(width > 0.0f);
    SP_ASSERT(height > 0.0f);

    if (m_viewSize.x == width && m_viewSize.y == height)
        return;

    m_viewSize = { width, height };

    m_projDirty = true;
    m_viewProjDirty = true;
}

// =========================================================
// Zoom
// =========================================================

void CameraComponent2D::SetZoom(float zoom)
{
    SP_ASSERT(zoom > 0.0f);

    if (fabsf(m_zoom - zoom) < 0.0001f)
        return;

    m_zoom = zoom;

    m_projDirty = true;
    m_viewProjDirty = true;
}

// =========================================================
// Near / Far
// =========================================================

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

// =========================================================
// Matrices
// =========================================================

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

// =========================================================
// SceneComponent
// =========================================================

void CameraComponent2D::OnTransformDirty()
{
    m_viewDirty = true;
    m_viewProjDirty = true;
}

// =========================================================
// Internal calculations
// =========================================================

void CameraComponent2D::RecalculateView() const
{
    const XMFLOAT3 pos = GetWorldPositionFast();

    // 2D Camera: rotation 무시
    m_view = XMMatrixTranslation(
        -pos.x,
        -pos.y,
        0.0f);

    m_viewDirty = false;
}

void CameraComponent2D::RecalculateProjection() const
{
    SP_ASSERT(IsViewSizeValid());
    SP_ASSERT(m_zoom > 0.0f);

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

    m_viewProjection = XMMatrixMultiply(m_view, m_projection);
    m_viewProjDirty = false;
}
