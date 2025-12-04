#pragma once

#include <DirectXMath.h>

struct SpriteConstantBuffer
{
    DirectX::XMMATRIX WVP;     // 64 bytes
    DirectX::XMFLOAT4 Color;   // 16 bytes (RGBA)

    DirectX::XMFLOAT2 TexCoord;  // 8 bytes
    DirectX::XMFLOAT2 TexSize;   // 8 bytes
};