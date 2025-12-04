#pragma once

#include <DirectXMath.h>

struct SpriteVertex
{
    DirectX::XMFLOAT3 position;   // float3
    DirectX::XMFLOAT4 color;      // float4
    DirectX::XMFLOAT2 texCoord;   // float2
};