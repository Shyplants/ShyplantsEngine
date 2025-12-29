#pragma once

#include "Engine/Core/Component/TransformComponent.h"

#include <DirectXMath.h>
#include <vector>

// Forward declarations
class CameraComponent2D;

/*
    WorldTransformComponent
    -------------------------------------------------
    - World-space Transform
    - Position / Rotation / Scale 관리
    - Transform Dirty 발생 시 CameraComponent2D에 알림
*/
class WorldTransformComponent final : public TransformComponent
{
public:
    explicit WorldTransformComponent(Actor* owner);
    ~WorldTransformComponent() override = default;

public:
    // =====================================================
    // Local transform (source of truth)
    // =====================================================
    void SetLocalPosition(const DirectX::XMFLOAT3& pos);
    void SetLocalRotation(const DirectX::XMFLOAT3& rot); // Euler (deg)
    void SetLocalScale(const DirectX::XMFLOAT3& scale);

    DirectX::XMFLOAT3 GetLocalPosition() const { return m_localPosition; }
    DirectX::XMFLOAT3 GetLocalRotation() const { return m_localRotation; }
    DirectX::XMFLOAT3 GetLocalScale()    const { return m_localScale; }

public:
    // =====================================================
    // Transform queries
    // =====================================================
    DirectX::XMMATRIX GetWorldMatrix() const override;
    DirectX::XMFLOAT3 GetWorldPositionFast() const override;

public:
    // =====================================================
    // Camera registration
    // =====================================================
    void RegisterCamera(CameraComponent2D* camera);
    void UnregisterCamera(CameraComponent2D* camera);

protected:
    // =====================================================
    // TransformComponent hook
    // =====================================================
    void OnTransformDirty() override;

private:
    // -------------------------------------------------
    // Local transform
    // -------------------------------------------------
    DirectX::XMFLOAT3 m_localPosition{ 0.f, 0.f, 0.f };
    DirectX::XMFLOAT3 m_localRotation{ 0.f, 0.f, 0.f };
    DirectX::XMFLOAT3 m_localScale{ 1.f, 1.f, 1.f };

    // -------------------------------------------------
    // Cached world matrix
    // -------------------------------------------------
    mutable DirectX::XMMATRIX m_cachedWorld =
        DirectX::XMMatrixIdentity();

    mutable bool m_worldDirty{ true };

    // -------------------------------------------------
    // Observers
    // -------------------------------------------------
    std::vector<CameraComponent2D*> m_cameras;
};
