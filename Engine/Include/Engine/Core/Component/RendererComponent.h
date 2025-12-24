#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Graphics/Render/RenderCategory.h"

#include <DirectXMath.h>

// Forward declarations
class SceneComponent;
class RenderQueue;
class CameraComponent2D;
class GraphicsPSO;
class MaterialInstance;
class Mesh;

/*
    RendererComponent
    -------------------------------------------------
    - Submit-only rendering component
    - DrawCommand를 생성하여 RenderQueue에 제출
    - 렌더링 정렬 정책(RenderCategory / RenderOrder) 소유
    - GPU 실행 책임 없음
*/
class RendererComponent : public ActorComponent
{
public:
    explicit RendererComponent(Actor* owner);
    ~RendererComponent() override = default;

    RendererComponent(const RendererComponent&) = delete;
    RendererComponent& operator=(const RendererComponent&) = delete;

    void OnRegister() override;

public:
    // =====================================================
    // Submit Interface
    // =====================================================
    virtual void SubmitWorld(
        RenderQueue& queue,
        const DirectX::XMMATRIX& viewProj) = 0;

    virtual void SubmitUI(
        RenderQueue& queue) 
    {
        // 기본 구현 없음 (UI Renderer에서만 사용)
    }

    virtual bool IsUIRenderer() const { return false; }

public:
    // =====================================================
    // Visibility
    // =====================================================
    void SetVisible(bool visible) { m_visible = visible; }
    bool IsVisible() const { return m_visible; }

public:
    // =====================================================
    // Attachment
    // =====================================================
    SceneComponent* GetAttachComponent() const { return m_attachComponent; }

public:
    // =====================================================
    // Render Category / Order
    // =====================================================
    void SetRenderCategory(ERenderCategory category) { m_category = category; }
    ERenderCategory GetRenderCategory() const { return m_category; }

    void SetRenderOrder(int32 order) { m_renderOrder = order; }
    int32 GetRenderOrder() const { return m_renderOrder; }

public:
    // =====================================================
    // RenderOffset
    // =====================================================
    void SetRenderOffset(const DirectX::XMFLOAT2& offset) { m_renderOffset = offset; }
    DirectX::XMFLOAT2 GetRenderOffset() const { return m_renderOffset; }

protected:
    friend class Actor;
    void SetAttachComponent(SceneComponent* comp)
    {
        m_attachComponent = comp;
    }

protected:
    // -------------------------------------------------
    // Attachment
    // -------------------------------------------------
    SceneComponent* m_attachComponent{ nullptr };

    // -------------------------------------------------
    // Visibility
    // -------------------------------------------------
    bool m_visible{ true };

    // -------------------------------------------------
    // Render policy
    // -------------------------------------------------
    ERenderCategory m_category{ ERenderCategory::World };
    int32           m_renderOrder{ 0 };

    // -------------------------------------------------
    // 2D offset (pixel / local)
    // -------------------------------------------------
    DirectX::XMFLOAT2 m_renderOffset{ 0.0f, 0.0f };
};
