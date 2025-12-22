#pragma once

#include "Common/Types.h"

/*
    SpriteShaderSlots
    -------------------------------------------------
    - SpriteVS / SpritePS 와 C++ 사이의 명확한 계약
    - Slot 변경 시 이 파일만 수정
*/

namespace SpriteShaderSlots
{
    // =========================================================
    // Textures (PS)
    // =========================================================
    inline constexpr uint32 DiffuseTexture = 0;

    // =========================================================
    // Samplers (PS)
    // =========================================================
    inline constexpr uint32 DiffuseSampler = 0;

    // =========================================================
    // Constant Buffers
    // =========================================================
    // b0
    inline constexpr uint32 PerObjectCB = 0;
}
