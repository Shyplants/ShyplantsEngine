#pragma once

#include "Common/Types.h"
#include <DirectXMath.h>

struct ScreenRenderContext
{
    // 실제 렌더링 해상도
    uint32 ViewportWidth{ 0 };
    uint32 ViewportHeight{ 0 };

    // DPI 배율 (1.0 = 96dpi 기준)
    float DPIScale{ 1.0f };

    // -----------------------------------
    // Helpers
    // -----------------------------------
    DirectX::XMFLOAT2 GetHalfViewport() const
    {
        return {
            ViewportWidth * 0.5f,
            ViewportHeight * 0.5f
        };
    }

    float ToScreenPixels(float logical) const
    {
        return logical * DPIScale;
    }
};
