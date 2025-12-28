#pragma once

#include "Engine/Core/Component/RendererComponent.h"

#include <DirectXMath.h>
#include <memory>
#include <unordered_map>

#include "Common/Math/Rect.h"
#include "Engine/Graphics/Render/SpritePivot.h"

// Forward declarations
class RenderQueue;
class SceneComponent;
class Material;
class MaterialInstance;
class Mesh;
class TextureResource;
class ConstantBuffer;

/*
    SpriteRendererComponent
    -------------------------------------------------
    - Sprite 렌더링 전용 RendererComponent
    - World / Screen 구분은 RenderCategory로 처리
    - 좌표 계산은 SceneComponent 결과를 그대로 사용
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

    void SubmitScreen(
        RenderQueue& queue) override;

public:
    // =====================================================
    // Policy
    // =====================================================
    void SetMaterial(Material* material);
    void SetMesh(Mesh* mesh);

    void SetTexture(uint32 slot, TextureResource* texture);

    void SetColor(const DirectX::XMFLOAT4& color);
    void SetAlpha(float alpha);
    void SetScale(const DirectX::XMFLOAT2& scale);
    void SetSourceRect(const Rect& rect);
    void SetPivot(SpritePivot pivot) { m_pivot = pivot; }

private:
    void EnsureMaterialInstance();
    void EnsureConstantBuffer();
    void ApplyCachedResources();

    void BuildDrawCommand(
        RenderQueue& queue,
        const DirectX::XMMATRIX& world,
        const DirectX::XMMATRIX& viewProj);

private:
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
    DirectX::XMFLOAT4 m_color{ 1.f, 1.f, 1.f, 1.f };
    Rect              m_sourceRect{};

    SpritePivot m_pivot{ SpritePivot::Center };
};
