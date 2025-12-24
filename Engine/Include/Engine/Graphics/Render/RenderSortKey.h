#pragma once
#include "Engine/Graphics/Render/RenderCategory.h"

inline uint32 MakeSortKey(
    ERenderCategory category,
    int32 order,
    float worldZ)
{
    // -------------------------------
    // 1. Clamp & pack Order (16bit)
    // -------------------------------
    uint32 orderKey =
        static_cast<uint32>(order) & 0x0000FFFF;

    // -------------------------------
    // 2. Normalize Z to 0~255
    // (가까울수록 앞에 그려지게)
    // -------------------------------
    constexpr float Z_MIN = -1000.0f;
    constexpr float Z_MAX = 1000.0f;

    float zNorm =
        (worldZ - Z_MIN) / (Z_MAX - Z_MIN);

    zNorm = std::clamp(zNorm, 0.0f, 1.0f);

    uint32 zKey =
        static_cast<uint32>(zNorm * 255.0f);

    // -------------------------------
    // 3. Pack SortKey
    // -------------------------------
    return (uint32(category) << 24) |
        (orderKey << 8) |
        (zKey);
}
