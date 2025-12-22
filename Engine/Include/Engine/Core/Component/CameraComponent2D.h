#pragma once

#include "Engine/Core/Component/SceneComponent.h"

#include <DirectXMath.h>

/*
    CameraComponent2D
    -------------------------------------------------
    - 2D Orthographic Camera
    - ViewSize는 반드시 외부에서 주입되어야 함
    - 초기 상태는 INVALID
*/
class CameraComponent2D final : public SceneComponent
{
public:
    explicit CameraComponent2D(Actor* owner);
    ~CameraComponent2D() override = default;

    CameraComponent2D(const CameraComponent2D&) = delete;
    CameraComponent2D& operator=(const CameraComponent2D&) = delete;

public:
    // =====================================================
    // View Size (Visible World Area)
    // =====================================================
    void SetViewSize(float width, float height);

    float GetViewWidth()  const { return m_viewSize.x; }
    float GetViewHeight() const { return m_viewSize.y; }

    bool IsViewSizeValid() const
    {
        return m_viewSize.x > 0.0f && m_viewSize.y > 0.0f;
    }

public:
    // =====================================================
    // Zoom (scale factor)
    // =====================================================
    void SetZoom(float zoom);
    float GetZoom() const { return m_zoom; }

public:
    // =====================================================
    // Near / Far
    // =====================================================
    void SetNearFar(float nearZ, float farZ);

public:
    // =====================================================
    // Matrices
    // =====================================================
    const DirectX::XMMATRIX& GetViewMatrix() const;
    const DirectX::XMMATRIX& GetProjectionMatrix() const;
    const DirectX::XMMATRIX& GetViewProjectionMatrix() const;

public:
    // =====================================================
    // SceneComponent
    // =====================================================
    void OnTransformDirty() override;

private:
    void RecalculateView() const;
    void RecalculateProjection() const;
    void RecalculateViewProjection() const;

private:
    // =====================================================
    // Camera parameters
    // =====================================================
    DirectX::XMFLOAT2 m_viewSize{ 0.0f, 0.0f }; // INVALID by default
    float m_zoom{ 1.0f };

    float m_nearZ{ 0.0f };
    float m_farZ{ 1000.0f };

private:
    // =====================================================
    // Cached matrices (mutable for const access)
    // =====================================================
    mutable DirectX::XMMATRIX m_view{};
    mutable DirectX::XMMATRIX m_projection{};
    mutable DirectX::XMMATRIX m_viewProjection{};

    mutable bool m_viewDirty{ true };
    mutable bool m_projDirty{ true };
    mutable bool m_viewProjDirty{ true };
};
