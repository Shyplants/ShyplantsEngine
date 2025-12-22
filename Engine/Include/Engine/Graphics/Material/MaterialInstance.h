#pragma once

#include <unordered_map>

#include "Common/Types.h"
#include "Engine/Graphics/Sampler/SamplerID.h"
#include "Engine/Graphics/Buffer/ConstantBuffer.h"

// Forward declarations
class Material;
class TextureResource;
class SamplerManager;

struct ID3D11Device;
struct ID3D11DeviceContext;

/*
    MaterialInstance
    -------------------------------------------------
    - Material의 per-draw 인스턴스
    - Texture / Sampler / ConstantBuffer 관리
    - Pipeline / PSO / GPU State에는 관여하지 않음
*/
class MaterialInstance
{
public:
    explicit MaterialInstance(Material* parent);
    ~MaterialInstance();

    MaterialInstance(const MaterialInstance&) = delete;
    MaterialInstance& operator=(const MaterialInstance&) = delete;

public:
    // =========================================================
    // Parent
    // =========================================================
    Material* GetMaterial() const { return m_parent; }

public:
    // =========================================================
    // Texture
    // =========================================================
    void SetTexture(uint32 slot, TextureResource* texture);

public:
    // =========================================================
    // Sampler (SamplerID 기반)
    // =========================================================
    void SetSampler(uint32 slot, SamplerID sampler);

public:
    // =========================================================
    // Constant Buffer
    // =========================================================
    void SetConstantBuffer(uint32 slot, ConstantBuffer* buffer);

    template<typename T>
    void UpdateConstantBuffer(uint32 slot, const T& data)
    {
        ConstantBuffer* buffer = GetConstantBuffer(slot);
        SP_ASSERT(buffer != nullptr);
        buffer->CacheCPUData(data);
    }

public:
    // =========================================================
    // Bind (Renderer 호출)
    // =========================================================
    void BindResources(
        SamplerManager& samplerManager,
        ID3D11Device* device,
        ID3D11DeviceContext* context) const;

private:
    ConstantBuffer* GetConstantBuffer(uint32 slot) const;

private:
    // =========================================================
    // Parent Material (non-owning)
    // =========================================================
    Material* m_parent{ nullptr };

private:
    // =========================================================
    // Per-instance resources
    // =========================================================
    std::unordered_map<uint32, TextureResource*> m_textures;
    std::unordered_map<uint32, SamplerID>        m_samplers;
    std::unordered_map<uint32, ConstantBuffer*>  m_constantBuffers;
};
