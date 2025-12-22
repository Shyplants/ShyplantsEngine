#pragma once

#include <Windows.h>
#include "Common/Math/Rect.h"

inline RECT ToWin32Rect(const Rect& r)
{
    return RECT{
        r.left,
        r.top,
        r.right,
        r.bottom
    };
}
