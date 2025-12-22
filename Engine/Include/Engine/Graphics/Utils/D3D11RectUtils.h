#pragma once

#include <d3d11.h>
#include "Common/Math/Rect.h"

inline D3D11_RECT ToD3DRect(const Rect& r)
{
    return D3D11_RECT{
        r.left,
        r.top,
        r.right,
        r.bottom
    };
}
