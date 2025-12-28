#pragma once

#include "Common/Types.h"

#include "Engine/Graphics/Render/RenderCategory.h"

/*
    RenderSortKey
    -------------------------------------------------
    64-bit 정렬 키 구조

    [63..62] RenderSpace
    [61..56] RenderLayer
    [55..40] RenderOrder
    [39..00] Depth (World only, inverted)

    - 작은 값일수록 먼저 렌더링됨
    - Screen Space는 Depth = 0
*/

using RenderSortKey = uint64;

// ------------------------------------------------------------
// Internal helpers
// ------------------------------------------------------------

namespace RenderSortKeyInternal
{
    /*
        DepthToBits
        -------------------------------------------------
        - float depth 값을 정렬 가능한 정수로 변환
        - 카메라에서 가까울수록 더 나중에 그려야 하므로 "반전"
        - NaN / Inf 방어 포함
    */
    inline uint32 DepthToBits(float depth)
    {
        // NaN 방어
        if (!std::isfinite(depth))
            depth = 0.0f;

        uint32 bits = 0;
        memcpy(&bits, &depth, sizeof(uint32));

        // IEEE754 float ordering trick
        // 음수/양수 정렬 보정
        if (bits & 0x80000000)
            bits = ~bits;
        else
            bits ^= 0x80000000;

        return bits;
    }
}

// ------------------------------------------------------------
// Public API
// ------------------------------------------------------------

/*
    MakeSortKey
    -------------------------------------------------
    - RendererComponent / SpriteRendererComponent에서 사용
*/
inline RenderSortKey MakeSortKey(
    ERenderSpace space,
    ERenderLayer layer,
    uint32 renderOrder,
    float depth = 0.0f)
{
    RenderSortKey key = 0;

    // -------------------------------------------------
    // RenderSpace (2 bits)
    // -------------------------------------------------
    key |= (static_cast<RenderSortKey>(space) & 0x3) << 62;

    // -------------------------------------------------
    // RenderLayer (6 bits)
    // -------------------------------------------------
    key |= (static_cast<RenderSortKey>(layer) & 0x3F) << 56;

    // -------------------------------------------------
    // RenderOrder (16 bits)
    // -------------------------------------------------
    key |= (static_cast<RenderSortKey>(renderOrder) & 0xFFFF) << 40;

    // -------------------------------------------------
    // Depth (40 bits)
    // -------------------------------------------------
    if (space == ERenderSpace::World)
    {
        uint32 depthBits =
            RenderSortKeyInternal::DepthToBits(depth);

        // 상위 40비트만 사용
        key |= static_cast<RenderSortKey>(depthBits);
    }

    return key;
}
