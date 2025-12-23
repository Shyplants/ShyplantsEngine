#pragma once

#include "Engine/Core/Component/ActorComponent.h"

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
        RenderQueue& queue) {
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
    SceneComponent* m_attachComponent{ nullptr };
    bool m_visible{ true };

    DirectX::XMFLOAT2 m_renderOffset{ 0.0f, 0.0f };
};
