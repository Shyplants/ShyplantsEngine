#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Graphics/Render/RenderCategory.h"

#include <DirectXMath.h>

class RenderQueue;

/*
    RendererComponent
    -------------------------------------------------
    - 순수 렌더링 Submit 컴포넌트
    - Transform / Camera / World / UI 개념 없음
    - Actor가 Submit 경로(World/UI)를 결정
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
    // Submit Interface (Actor가 호출)
    // =====================================================

    // World-space rendering
    virtual void SubmitWorld(
        RenderQueue& queue,
        const DirectX::XMMATRIX& viewProj) = 0;

    // Screen-space rendering (UI)
    virtual void SubmitUI(RenderQueue& /*queue*/) {}

public:
    // =====================================================
    // Visibility
    // =====================================================
    void SetVisible(bool visible) { m_visible = visible; }
    bool IsVisible() const { return m_visible; }

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

    bool IsScreenSpace() const
    {
        return m_category.Space == ERenderSpace::Screen;
    }

    bool IsWorldSpace() const
    {
        return m_category.Space == ERenderSpace::World;
    }

    void SetRenderOrder(int32 order)
    {
        m_renderOrder = order;
    }

    int32 GetRenderOrder() const
    {
        return m_renderOrder;
    }

protected:
    // =====================================================
    // Helper
    // =====================================================
    bool CanRender() const
    {
        return m_visible && IsActive();
    }

protected:
    bool m_visible{ true };

    RenderCategory m_category{};
    int32          m_renderOrder{ 0 };
};
