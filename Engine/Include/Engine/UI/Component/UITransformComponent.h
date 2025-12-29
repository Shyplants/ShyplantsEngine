#pragma once

#include "Engine/Core/Component/TransformComponent.h"
#include "Engine/UI/UISpaceContext.h"
#include "Engine/UI/Layout/UIAnchor.h"

#include <DirectXMath.h>

// Forward declarations
class UIScreenRootComponent;

/*
    UITransformComponent
    -------------------------------------------------
    - Screen-space 전용 Transform
    - Anchor + Offset + UISpaceContext 기반
*/
class UITransformComponent final : public TransformComponent
{
public:
    explicit UITransformComponent(Actor* owner);
    ~UITransformComponent() override;

public:
    // =====================================================
    // Layout configuration
    // =====================================================
    void SetAnchor(const UIAnchorData& anchor);
    void SetLocalOffset(const DirectX::XMFLOAT2& offset);

    void MarkLayoutDirty();

    // Root에서만 호출
    void SetUISpace(const UISpaceContext* space);

public:
    // =====================================================
    // TransformComponent override
    // =====================================================
    DirectX::XMMATRIX GetWorldMatrix() const override;
    DirectX::XMFLOAT3 GetWorldPositionFast() const override;

public:
    // =====================================================
    // ActorComponent lifecycle
    // =====================================================
    void OnRegister() override;
    void OnUnregister() override;

protected:
    // =====================================================
    // Transform dirty hook
    // =====================================================
    void OnTransformDirty() override;

private:
    void ResolveRoot();
    void Recalculate() const;

private:
    // -------------------------------------------------
    // UISpace
    // -------------------------------------------------
    const UISpaceContext* m_space{ nullptr };
    UIScreenRootComponent* m_root{ nullptr };

    // -------------------------------------------------
    // Layout state
    // -------------------------------------------------
    UIAnchorData m_anchor{};
    DirectX::XMFLOAT2 m_localOffset{ 0.f, 0.f };

    // -------------------------------------------------
    // Cached result
    // -------------------------------------------------
    mutable DirectX::XMFLOAT2 m_cachedPos{ 0.f, 0.f };
    mutable bool m_dirty{ true };
};
