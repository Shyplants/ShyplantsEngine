#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Material/MaterialInstance.h"
#include "Engine/Graphics/Material/Material.h"
#include "Engine/Graphics/Sampler/SamplerManager.h"
#include "Engine/Resource/Texture/TextureResource.h"

// =========================================================
// Constructor / Destructor
// =========================================================

MaterialInstance::MaterialInstance(Material* parent)
    : m_parent(parent)
{
    SP_ASSERT(parent != nullptr);
}

MaterialInstance::~MaterialInstance() = default;

// =========================================================
// Texture
// =========================================================

void MaterialInstance::SetTexture(
    uint32 slot,
    TextureResource* texture)
{
    SP_ASSERT(texture != nullptr);
    m_textures[slot] = texture;
}

// =========================================================
// Sampler
// =========================================================

void MaterialInstance::SetSampler(
    uint32 slot,
    SamplerID sampler)
{
    SP_ASSERT(sampler.IsValid());
    m_samplers[slot] = sampler;
}

// =========================================================
// Constant Buffer
// =========================================================

void MaterialInstance::SetConstantBuffer(
    uint32 slot,
    ConstantBuffer* buffer)
{
    SP_ASSERT(buffer != nullptr);
    m_constantBuffers[slot] = buffer;
}

ConstantBuffer* MaterialInstance::GetConstantBuffer(uint32 slot) const
{
    auto it = m_constantBuffers.find(slot);
    return (it != m_constantBuffers.end()) ? it->second : nullptr;
}

// =========================================================
// Bind
// =========================================================

void MaterialInstance::BindResources(
    SamplerManager& samplerManager,
    ID3D11Device* device,
    ID3D11DeviceContext* context) const
{
    SP_ASSERT(context != nullptr);

    // -------------------------------------------------
    // Textures (SRV)
    // -------------------------------------------------
    for (const auto& [slot, texture] : m_textures)
    {
        ID3D11ShaderResourceView* srv =
            texture ? texture->GetSRV() : nullptr;

        context->PSSetShaderResources(slot, 1, &srv);
    }

    // -------------------------------------------------
    // Samplers (SamplerID ¡æ ID3D11SamplerState)
    // -------------------------------------------------
    for (const auto& [slot, samplerID] : m_samplers)
    {
        ID3D11SamplerState* sampler =
            samplerManager.Resolve(samplerID);

        context->PSSetSamplers(slot, 1, &sampler);
    }

    // -------------------------------------------------
    // Constant Buffers
    // -------------------------------------------------
    for (const auto& [slot, buffer] : m_constantBuffers)
    {
        buffer->UploadIfDirty(device, context);

        ID3D11Buffer* d3d = buffer->GetBuffer();
        context->VSSetConstantBuffers(slot, 1, &d3d);
        context->PSSetConstantBuffers(slot, 1, &d3d);
    }
}
