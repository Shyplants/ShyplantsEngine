#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/FAttachmentTransformRules.h"

#include <DirectXMath.h>
#include <vector>

// Forward declaration
class Actor;

class SceneComponent : public ActorComponent
{
public:
    explicit SceneComponent(Actor* owner);
    ~SceneComponent() override;

    void OnRegister() override;

public:
    // =====================================================
    // Local Transform (Source of Truth)
    // =====================================================
    void SetLocalPosition(const DirectX::XMFLOAT3& pos);
    void SetLocalRotation(const DirectX::XMFLOAT3& rot); // Euler (deg)
    void SetLocalScale(const DirectX::XMFLOAT3& scale);

    DirectX::XMFLOAT3 GetLocalPosition() const { return m_localPosition; }
    DirectX::XMFLOAT3 GetLocalRotation() const { return m_localRotation; }
    DirectX::XMFLOAT3 GetLocalScale() const { return m_localScale; }

public:
    // =====================================================
    // World Transform Query
    // =====================================================
    DirectX::XMFLOAT3 GetWorldPosition();
    DirectX::XMFLOAT3 GetWorldPositionFast() const;
    DirectX::XMFLOAT3 GetWorldScale();

    DirectX::XMMATRIX GetLocalMatrix() const;
    DirectX::XMMATRIX GetWorldMatrix() const;

public:
    // =====================================================
    // Attachment (PUBLIC / SAFE)
    // =====================================================
    void AttachTo(
        SceneComponent* parent,
        const FAttachmentTransformRules& rules =
        FAttachmentTransformRules::KeepRelativeTransform);

    SceneComponent* GetParent() const { return m_parent; }
    const std::vector<SceneComponent*>& GetChildren() const { return m_children; }

protected:
    // =====================================================
    // Attachment (INTERNAL / UNSAFE)
    // =====================================================
    // Unsafe: assumes owner consistency & valid hierarchy
    friend class Actor;
    void AttachToUnsafe(
        SceneComponent* parent,
        const FAttachmentTransformRules& rules);

    void DetachUnsafe(
        const FDetachmentTransformRules& rules);

protected:
    // =====================================================
    // Hierarchy helpers (Actor / Scene only)
    // =====================================================
    void AddChild(SceneComponent* child);
    void RemoveChild(SceneComponent* child);

protected:
    // =====================================================
    // Attachment Events (EMPTY HOOKS)
    // =====================================================
    // Called AFTER attachment has completed
    virtual void OnAttached(SceneComponent* newParent) {}

    // Called AFTER detachment has completed
    virtual void OnDetached(SceneComponent* oldParent) {}

protected:
    // =====================================================
    // Dirty Notification Hook
    // =====================================================
    virtual void OnTransformDirty() {}

private:
    void MarkDirtyRecursive();

private:
    // =====================================================
    // Hierarchy
    // =====================================================
    SceneComponent* m_parent{ nullptr };
    std::vector<SceneComponent*> m_children;

    // =====================================================
    // Local Transform (Euler, deg)
    // =====================================================
    DirectX::XMFLOAT3 m_localPosition{ 0.f, 0.f, 0.f };
    DirectX::XMFLOAT3 m_localRotation{ 0.f, 0.f, 0.f };
    DirectX::XMFLOAT3 m_localScale{ 1.f, 1.f, 1.f };

    // =====================================================
    // Cached World Transform
    // =====================================================
    mutable DirectX::XMMATRIX m_cachedWorldMatrix =
        DirectX::XMMatrixIdentity();

    mutable bool m_worldDirty{ true };
};
