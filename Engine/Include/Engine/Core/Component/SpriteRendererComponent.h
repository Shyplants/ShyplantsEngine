#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "Common/Math/Rect.h"
#include "Engine/Graphics/Render/SpritePivot.h"

#include <DirectXMath.h>
#include <memory>
#include <unordered_map>

// Forward declarations
class RenderQueue;
class TransformComponent;

class Material;
class MaterialInstance;
class Mesh;
class TextureResource;
class ConstantBuffer;

/*
    SpriteRendererComponent
    -------------------------------------------------
    - World / Screen 공용 Sprite Renderer
    - TransformComponent 추상 인터페이스만 사용
    - Sprite 전용 조립 파라미터(Pivot / Scale / RenderOffset)를 소유
*/
class SpriteRendererComponent final : public RendererComponent
{
public:
    explicit SpriteRendererComponent(Actor* owner);
    ~SpriteRendererComponent() override;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    void OnRegister() override;

public:
    // =====================================================
    // Submit
    // =====================================================
    void SubmitWorld(
        RenderQueue& queue,
        const DirectX::XMMATRIX& viewProj) override;

    void SubmitUI(
        RenderQueue& queue) override;

public:
    // =====================================================
    // Policy (Sprite state)
    // =====================================================
    void SetMaterial(Material* material);
    void SetMesh(Mesh* mesh);

    void SetTexture(uint32 slot, TextureResource* texture);

    void SetColor(const DirectX::XMFLOAT4& color);
    void SetAlpha(float alpha);
    void SetScale(const DirectX::XMFLOAT2& scale);

    void SetRenderOffset(const DirectX::XMFLOAT2& offset);
    const DirectX::XMFLOAT2& GetRenderOffset() const;

    void SetSourceRect(const Rect& rect);
    void SetPivot(SpritePivot pivot) { m_pivot = pivot; }

private:
    // =====================================================
    // Internal
    // =====================================================
    void EnsureMaterialInstance();
    void EnsureConstantBuffer();
    void ApplyCachedResources();

    void BuildDrawCommand(
        RenderQueue& queue,
        const DirectX::XMMATRIX& world,
        const DirectX::XMMATRIX& viewProj);

private:
    // -------------------------------------------------
    // Cached Transform
    // -------------------------------------------------
    TransformComponent* m_transform{ nullptr };

    // -------------------------------------------------
    // Render resources
    // -------------------------------------------------
    Material* m_material{ nullptr };
    Mesh* m_mesh{ nullptr };

    std::unique_ptr<MaterialInstance> m_materialInstance;
    std::unique_ptr<ConstantBuffer>   m_spriteCB;

    std::unordered_map<uint32, TextureResource*> m_textures;
    TextureResource* m_activeTexture{ nullptr };

    // -------------------------------------------------
    // Sprite state
    // -------------------------------------------------
    DirectX::XMFLOAT2 m_scale{ 1.f, 1.f };
    DirectX::XMFLOAT2 m_renderOffset{ 0.f, 0.f };

    DirectX::XMFLOAT4 m_color{ 1.f, 1.f, 1.f, 1.f };
    Rect              m_sourceRect{};

    SpritePivot m_pivot{ SpritePivot::Center };
};
