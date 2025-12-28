#pragma once

#include <DirectXMath.h>

inline DirectX::XMMATRIX
MakeScreenProjection(float width, float height)
{
    const float halfW = width * 0.5f;
    const float halfH = height * 0.5f;

    return DirectX::XMMatrixOrthographicOffCenterLH(
        -halfW, halfW,
        -halfH, halfH,
        0.0f, 1.0f);
}
