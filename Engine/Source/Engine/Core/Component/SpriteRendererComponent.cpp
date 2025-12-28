#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"

#include "Engine/Graphics/Render/RenderSystem.h"
#include "Engine/Graphics/Render/RenderQueue.h"
#include "Engine/Graphics/Render/DrawCommand.h"
#include "Engine/Graphics/Render/RenderSortKey.h"

#include "Engine/Graphics/Material/Material.h"
#include "Engine/Graphics/Material/MaterialInstance.h"
#include "Engine/Graphics/Mesh/Mesh.h"

#include "Engine/Graphics/Buffer/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffer/SpriteConstantBuffer.h"

#include "Engine/Resource/Texture/TextureResource.h"
#include "Engine/Graphics/Sampler/SamplerID.h"
#include "Engine/Graphics/Shader/SpriteShaderSlots.h"

using namespace DirectX;
using namespace SpriteShaderSlots;

// =====================================================
// Constructor / Destructor
// =====================================================

SpriteRendererComponent::SpriteRendererComponent(Actor* owner)
    : RendererComponent(owner)
{
    SP_ASSERT(owner != nullptr);
}

SpriteRendererComponent::~SpriteRendererComponent() = default;

// =====================================================
// Lifecycle
// =====================================================

void SpriteRendererComponent::OnRegister()
{
    RendererComponent::OnRegister();

    auto& defaults =
        GetWorld()->GetRenderSystem().GetDefaults();

    if (!m_material)
        m_material = defaults.SpriteMaterial;

    if (!m_mesh)
        m_mesh = defaults.SpriteMesh;

    EnsureMaterialInstance();
    EnsureConstantBuffer();
    ApplyCachedResources();
}

// =====================================================
// Submit
// =====================================================

void SpriteRendererComponent::SubmitWorld(
    RenderQueue& queue,
    const XMMATRIX& viewProj)
{
    if (!IsVisible())
        return;

    SceneComponent* scene = GetAttachComponent();
    SP_ASSERT(scene);

    XMMATRIX world = scene->GetWorldMatrix();
    BuildDrawCommand(queue, world, viewProj);
}

void SpriteRendererComponent::SubmitScreen(RenderQueue& queue)
{
    if (!IsVisible())
        return;

    SceneComponent* scene = GetAttachComponent();
    SP_ASSERT(scene);

    XMMATRIX world = scene->GetWorldMatrix();
    BuildDrawCommand(queue, world, XMMatrixIdentity());
}

// =====================================================
// DrawCommand builder
// =====================================================

void SpriteRendererComponent::BuildDrawCommand(
    RenderQueue& queue,
    const DirectX::XMMATRIX& world,
    const DirectX::XMMATRIX& viewProj)
{
    SP_ASSERT(m_material);
    SP_ASSERT(m_materialInstance);
    SP_ASSERT(m_mesh);
    SP_ASSERT(m_activeTexture);

    float spriteW = float(m_sourceRect.right - m_sourceRect.left);
    float spriteH = float(m_sourceRect.bottom - m_sourceRect.top);

    XMFLOAT2 pivotPixel{ 0.f, 0.f };

    switch (m_pivot)
    {
    case SpritePivot::TopLeft:      pivotPixel = { 0.f, spriteH }; break;
    case SpritePivot::TopCenter:    pivotPixel = { spriteW * 0.5f, spriteH }; break;
    case SpritePivot::TopRight:     pivotPixel = { spriteW, spriteH }; break;
    case SpritePivot::MiddleLeft:   pivotPixel = { 0.f, spriteH * 0.5f }; break;
    case SpritePivot::Center:
    case SpritePivot::MiddleCenter: pivotPixel = { spriteW * 0.5f, spriteH * 0.5f }; break;
    case SpritePivot::MiddleRight:  pivotPixel = { spriteW, spriteH * 0.5f }; break;
    case SpritePivot::BottomLeft:   pivotPixel = { 0.f, 0.f }; break;
    case SpritePivot::BottomCenter: pivotPixel = { spriteW * 0.5f, 0.f }; break;
    case SpritePivot::BottomRight:  pivotPixel = { spriteW, 0.f }; break;
    }

    XMMATRIX scale = XMMatrixScaling(
        spriteW * m_scale.x,
        spriteH * m_scale.y,
        1.f);

    XMMATRIX pivot = XMMatrixTranslation(
        -pivotPixel.x,
        -pivotPixel.y,
        0.f);

    XMMATRIX offset = XMMatrixTranslation(
        m_renderOffset.x,
        m_renderOffset.y,
        0.f);

    XMMATRIX finalWorld = scale * pivot * offset * world;

    // -------------------------------------------------
    // Constant buffer
    // -------------------------------------------------
    SpriteConstantBuffer cb{};
    cb.WVP = XMMatrixTranspose(finalWorld * viewProj);
    cb.Color = m_color;

    float invW = 1.f / m_activeTexture->GetWidth();
    float invH = 1.f / m_activeTexture->GetHeight();

    cb.UVRect = XMFLOAT4(
        m_sourceRect.left * invW,
        m_sourceRect.top * invH,
        (m_sourceRect.right - m_sourceRect.left) * invW,
        (m_sourceRect.bottom - m_sourceRect.top) * invH);

    m_materialInstance->UpdateConstantBuffer(
        PerObjectCB, cb);

    // -------------------------------------------------
    // DrawCommand
    // -------------------------------------------------
    DrawCommand cmd{};
    cmd.Pipeline = m_material->GetPipeline();
    cmd.MaterialInstance = m_materialInstance.get();
    cmd.Mesh = m_mesh;

    cmd.IndexCount = m_mesh->GetIndexCount();
    cmd.SortKey = MakeSortKey(
        m_category.Space,
        m_category.Layer,
        static_cast<uint32>(m_renderOrder),
        GetAttachComponent()->GetWorldPositionFast().z);

    queue.Submit(cmd);
}

// =====================================================
// Policy
// =====================================================

void SpriteRendererComponent::SetMaterial(Material* material)
{
    SP_ASSERT(material);
    m_material = material;
    m_materialInstance.reset();

    EnsureMaterialInstance();
    ApplyCachedResources();
}

void SpriteRendererComponent::SetMesh(Mesh* mesh)
{
    SP_ASSERT(mesh);
    m_mesh = mesh;
}

void SpriteRendererComponent::SetTexture(
    uint32 slot,
    TextureResource* texture)
{
    SP_ASSERT(texture);

    m_textures[slot] = texture;
    m_activeTexture = texture;

    if (m_sourceRect.IsEmpty())
    {
        m_sourceRect = {
            0, 0,
            static_cast<int32>(texture->GetWidth()),
            static_cast<int32>(texture->GetHeight())
        };
    }

    if (m_materialInstance)
        m_materialInstance->SetTexture(slot, texture);
}

void SpriteRendererComponent::SetColor(const XMFLOAT4& color)
{
    m_color = color;
}

void SpriteRendererComponent::SetAlpha(float alpha)
{
    m_color.w = alpha;
}

void SpriteRendererComponent::SetScale(const XMFLOAT2& scale)
{
    m_scale = scale;
}

void SpriteRendererComponent::SetSourceRect(const Rect& rect)
{
    m_sourceRect = rect;
}

// =====================================================
// Internal helpers
// =====================================================

void SpriteRendererComponent::EnsureMaterialInstance()
{
    if (m_materialInstance)
        return;

    m_materialInstance =
        std::make_unique<MaterialInstance>(m_material);

    m_materialInstance->SetSampler(
        DiffuseSampler,
        BuiltinSampler::LinearClamp);
}

void SpriteRendererComponent::EnsureConstantBuffer()
{
    if (m_spriteCB)
        return;

    m_spriteCB = std::make_unique<ConstantBuffer>(
        static_cast<uint32>(sizeof(SpriteConstantBuffer)),
        ConstantBufferUsage::Dynamic);
}

void SpriteRendererComponent::ApplyCachedResources()
{
    for (const auto& [slot, tex] : m_textures)
        m_materialInstance->SetTexture(slot, tex);

    m_materialInstance->SetConstantBuffer(
        PerObjectCB,
        m_spriteCB.get());
}
