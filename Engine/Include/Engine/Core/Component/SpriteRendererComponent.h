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
    - Sprite 상태의 Source of Truth
    - Material은 Pipeline 정책 제공자
    - SubmitWorld는 DrawCommand 생성만 수행
*/
class SpriteRendererComponent final : public RendererComponent
{
public:
    explicit SpriteRendererComponent(Actor* owner);
    ~SpriteRendererComponent() override;

public:
    void OnRegister() override;

public:
    void SubmitWorld(
        RenderQueue& queue,
        const DirectX::XMMATRIX& viewProj) override;

public:
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

private:
    Material* m_material{ nullptr };
    Mesh* m_mesh{ nullptr };

    std::unique_ptr<MaterialInstance> m_materialInstance;
    std::unique_ptr<ConstantBuffer>   m_spriteCB;

    std::unordered_map<uint32, TextureResource*> m_textures;
    TextureResource* m_activeTexture{ nullptr };

    DirectX::XMFLOAT2 m_scale{ 1.f, 1.f };
    DirectX::XMFLOAT4 m_color{ 1.f, 1.f, 1.f, 1.f };
    Rect              m_sourceRect{};

    SpritePivot m_pivot{ SpritePivot::Center };
};
