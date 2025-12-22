#pragma once

#include <DirectXMath.h>

// GPU Constant Buffer (Sprite)
struct alignas(16) SpriteConstantBuffer
{
    // =====================================================
    // Transform
    // =====================================================
    DirectX::XMMATRIX WVP;        // World * View * Projection

    // =====================================================
    // Appearance
    // =====================================================
    DirectX::XMFLOAT4 Color;      // RGBA (Alpha Æ÷ÇÔ)

    // =====================================================
    // UV Control
    // =====================================================
    DirectX::XMFLOAT4 UVRect;     // (u, v, width, height) [0~1]

    // =====================================================
    // Padding (16-byte alignment)
    // =====================================================
    DirectX::XMFLOAT4 _Padding{ 0,0,0,0 };
};