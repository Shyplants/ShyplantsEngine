#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Graphics/Render/RenderCategory.h"

#include <DirectXMath.h>

// Forward declarations
class SceneComponent;
class RenderQueue;
class CameraComponent2D;

/*
    RendererComponent
    -------------------------------------------------
    - Submit-only rendering component
    - DrawCommand를 생성하여 RenderQueue에 제출
    - GPU 실행 책임 없음
    - RenderCategory : 정렬 정책
    - RenderSpace    : World / Screen 좌표 정책
*/
class RendererComponent : public ActorComponent
{
public:
    explicit RendererComponent(Actor* owner);
    ~RendererComponent() override = default;

    RendererComponent(const RendererComponent&) = delete;
    RendererComponent& operator=(const RendererComponent&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    void OnRegister() override;

public:
    // =====================================================
    // Submit Interface
    // =====================================================
    void Submit(
        RenderQueue& queue,
        CameraComponent2D* activeCamera);

protected:
    // =====================================================
    // Space-specific hooks (파생 클래스 구현)
    // =====================================================
    virtual void SubmitWorld(
        RenderQueue& queue,
        const DirectX::XMMATRIX& viewProj) 
    {}

    virtual void SubmitScreen(
        RenderQueue& queue) 
    {}
    

public:
    // =====================================================
    // Render Space
    // =====================================================
    bool IsScreenSpace() const
    {
        return m_category.Space == ERenderSpace::Screen;
    }

    bool IsWorldSpace() const
    {
        return m_category.Space == ERenderSpace::World;
    }

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
    SceneComponent* GetAttachComponent() const
    {
        return m_attachComponent;
    }

public:
    // =====================================================
    // Render Category / Order
    // =====================================================
    void SetRenderCategory(const RenderCategory& category)
    {
        m_category = category;
    }

    const RenderCategory& GetRenderCategory() const
    {
        return m_category;
    }

    void SetRenderOrder(int32 order)
    {
        m_renderOrder = order;
    }

    int32 GetRenderOrder() const
    {
        return m_renderOrder;
    }

public:
    // =====================================================
    // Render Offset (local 2D pixel offset)
    // =====================================================
    void SetRenderOffset(const DirectX::XMFLOAT2& offset)
    {
        m_renderOffset = offset;
    }

    DirectX::XMFLOAT2 GetRenderOffset() const
    {
        return m_renderOffset;
    }

protected:
    // Actor만 Attachment를 설정 가능
    friend class Actor;

    void SetAttachComponent(SceneComponent* component)
    {
        m_attachComponent = component;
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
    RenderCategory m_category{};
    int32          m_renderOrder{ 0 };

    // -------------------------------------------------
    // Local 2D render offset (pixel)
    // -------------------------------------------------
    DirectX::XMFLOAT2 m_renderOffset{ 0.0f, 0.0f };
};
