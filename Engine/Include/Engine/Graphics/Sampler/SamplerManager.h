#pragma once

#include <unordered_map>
#include <memory>

#include <d3d11.h>
#include <wrl/client.h>

#include "Engine/Graphics/Sampler/SamplerID.h"
#include "Engine/Graphics/Sampler/SamplerDesc.h"

// Forward declarations
class RenderDevice;

/*
    SamplerManager
    -------------------------------------------------
    - ID3D11SamplerState 생성 / 캐시 / 조회
    - Built-in Sampler를 엔진 시작 시 등록
*/
class SamplerManager
{
public:
    explicit SamplerManager(RenderDevice* device);
    ~SamplerManager();

    SamplerManager(const SamplerManager&) = delete;
    SamplerManager& operator=(const SamplerManager&) = delete;

public:
    // =========================================================
    // Initialize
    // =========================================================
    void Initialize();

public:
    // =========================================================
    // Access
    // =========================================================
    ID3D11SamplerState* Resolve(SamplerID id) const;

private:
    void RegisterBuiltinSamplers();

private:
    RenderDevice* m_device{ nullptr };

    std::unordered_map<
        SamplerID,
        Microsoft::WRL::ComPtr<ID3D11SamplerState>,
        SamplerIDHash
    > m_samplers;
};
