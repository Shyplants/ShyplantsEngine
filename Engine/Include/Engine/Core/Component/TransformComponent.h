#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include <vector>
#include <DirectXMath.h>

/*
    TransformComponent
    -------------------------------------------------
    - Transform 계층의 공통 베이스
    - 계층(Hierarchy) 관리 + Dirty 전파
    - World/UI 각 좌표계의 Query는 파생 클래스가 구현
*/
class TransformComponent : public ActorComponent
{
public:
    explicit TransformComponent(Actor* owner);
    ~TransformComponent() override;

public:
    // =====================================================
    // Hierarchy
    // =====================================================
    TransformComponent* GetParent() const { return m_parent; }
    const std::vector<TransformComponent*>& GetChildren() const;

    void AttachTo(TransformComponent* parent);
    void Detach();

public:
    // =====================================================
    // Transform Query (POLYMORPHIC)
    // - WorldTransformComponent / UITransformComponent 가 구현
    // =====================================================
    virtual DirectX::XMMATRIX GetWorldMatrix() const = 0;
    virtual DirectX::XMFLOAT3 GetWorldPositionFast() const = 0;

protected:
    // =====================================================
    // Hooks
    // =====================================================
    virtual void OnAttached(TransformComponent* parent) {}
    virtual void OnDetached() {}

    // Dirty 전파 시 호출됨 (파생에서 캐시 invalidation)
    virtual void OnTransformDirty() {}

protected:
    // =====================================================
    // Dirty propagation
    // =====================================================
    void MarkDirtyRecursive();

private:
    void AddChild(TransformComponent* child);
    void RemoveChild(TransformComponent* child);

protected:
    TransformComponent* m_parent{ nullptr };
    std::vector<TransformComponent*> m_children;
};
