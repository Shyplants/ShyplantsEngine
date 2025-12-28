#pragma once

#include <DirectXMath.h>

#include "Engine/UI/Components/UIComponent.h"
#include "Engine/UI/Layout/UIAnchor.h"

// Forward declarations
class UIScreenRootComponent;

/*
    UIScreenTransform
    -------------------------------------------------
    - Screen-space UI 노드
    - UI 트리 등록/해제는 명시적 lifecycle 훅에서 수행
*/
class UIScreenTransform final : public UIComponent
{
public:
    explicit UIScreenTransform(Actor* owner);
    ~UIScreenTransform() override;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    void OnRegister() override;
    void OnUnregister() override;

public:
    // =====================================================
    // Root binding
    // =====================================================
    void SetRoot(UIScreenRootComponent* root);
    UIScreenRootComponent* GetRoot() const { return m_root; }

public:
    // =====================================================
    // Layout
    // =====================================================
    void SetAnchor(const UIAnchorData& anchor);
    void SetLocalOffset(float x, float y);

    void MarkLayoutDirty();

    DirectX::XMFLOAT2 GetScreenPosition() const;

private:
    void Recalculate() const;

private:
    UIScreenRootComponent* m_root{ nullptr };

    UIAnchorData m_anchor{};
    DirectX::XMFLOAT2 m_localOffset{ 0.0f, 0.0f };

    mutable DirectX::XMFLOAT2 m_cachedPos{};
    mutable bool m_dirty{ true };
};
