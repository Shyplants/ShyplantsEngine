#pragma once

#include <d3d11.h>
#include <cstring>

#include "Common/Types.h"

struct SamplerDesc
{
    D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    float MipLODBias = 0.0f;
    uint32 MaxAnisotropy = 1;

    D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER;

    float BorderColor[4] = { 0,0,0,0 };

    float MinLOD = 0.0f;
    float MaxLOD = D3D11_FLOAT32_MAX;

    inline bool operator==(const SamplerDesc& rhs) const
    {
        return std::memcmp(this, &rhs, sizeof(SamplerDesc)) == 0;
    }
};
