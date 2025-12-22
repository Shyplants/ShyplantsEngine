#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Sampler/SamplerManager.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"

// =========================================================
// Constructor / Destructor
// =========================================================

SamplerManager::SamplerManager(RenderDevice* device)
    : m_device(device)
{
    SP_ASSERT(m_device != nullptr);
}

SamplerManager::~SamplerManager() = default;

// =========================================================
// Initialize
// =========================================================

void SamplerManager::Initialize()
{
    RegisterBuiltinSamplers();

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "SamplerManager initialized");
#endif
}

// =========================================================
// Built-in Samplers
// =========================================================

void SamplerManager::RegisterBuiltinSamplers()
{
    ID3D11Device* device = m_device->GetDevice();
    SP_ASSERT(device != nullptr);

    /*
        LinearClamp
        -------------------------------------------------
        - Default Sprite / UI Sampler
    */
    {
        D3D11_SAMPLER_DESC desc{};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
        HRESULT hr = device->CreateSamplerState(&desc, sampler.GetAddressOf());
        SP_ASSERT(SUCCEEDED(hr));

        m_samplers.emplace(BuiltinSampler::LinearClamp, sampler);
    }

    /*
        PointWrap
        -------------------------------------------------
        - Pixel art / Debug / Nearest
    */
    {
        D3D11_SAMPLER_DESC desc{};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
        HRESULT hr = device->CreateSamplerState(&desc, sampler.GetAddressOf());
        SP_ASSERT(SUCCEEDED(hr));

        m_samplers.emplace(BuiltinSampler::PointWrap, sampler);
    }
}

// =========================================================
// Resolve
// =========================================================

ID3D11SamplerState*
SamplerManager::Resolve(SamplerID id) const
{
    auto it = m_samplers.find(id);
    SP_ASSERT(it != m_samplers.end());

    return it->second.Get();
}
