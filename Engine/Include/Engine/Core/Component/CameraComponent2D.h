#pragma once

#include "Engine/Core/Component/ActorComponent.h"

#include <DirectXMath.h>

// Forward declarations
class WorldTransformComponent;

/*
    CameraComponent2D
    -------------------------------------------------
    - 2D Orthographic Camera
    - WorldTransformComponent 기반
    - Transform Dirty 연동 최적화
*/
class CameraComponent2D final : public ActorComponent
{
public:
    explicit CameraComponent2D(Actor* owner);
    ~CameraComponent2D() override;

    CameraComponent2D(const CameraComponent2D&) = delete;
    CameraComponent2D& operator=(const CameraComponent2D&) = delete;

public:
    // =====================================================
    // View size
    // =====================================================
    void SetViewSize(float width, float height);

    bool IsViewSizeValid() const
    {
        return m_viewSize.x > 0.f && m_viewSize.y > 0.f;
    }

public:
    // =====================================================
    // Camera parameters
    // =====================================================
    void SetZoom(float zoom);
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
    // Transform Dirty notification
    // =====================================================
    void OnOwnerTransformDirty();

public:
    // =====================================================
    // ActorComponent lifecycle
    // =====================================================
    void OnRegister() override;
    void OnUnregister() override;

private:
    void RecalculateView() const;
    void RecalculateProjection() const;
    void RecalculateViewProjection() const;

private:
    // -------------------------------------------------
    // Dependencies
    // -------------------------------------------------
    WorldTransformComponent* m_transform{ nullptr };

    // -------------------------------------------------
    // Parameters
    // -------------------------------------------------
    DirectX::XMFLOAT2 m_viewSize{ 0.f, 0.f };
    float m_zoom{ 1.f };
    float m_nearZ{ 0.f };
    float m_farZ{ 1000.f };

    // -------------------------------------------------
    // Cached matrices
    // -------------------------------------------------
    mutable DirectX::XMMATRIX m_view{};
    mutable DirectX::XMMATRIX m_projection{};
    mutable DirectX::XMMATRIX m_viewProjection{};

    mutable bool m_viewDirty{ true };
    mutable bool m_projDirty{ true };
    mutable bool m_viewProjDirty{ true };
};
