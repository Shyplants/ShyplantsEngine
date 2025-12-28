#pragma once

#include <cstring>

#include "Common/Types.h"

/*
    UISafeArea
    -------------------------------------------------
    - 플랫폼별 Safe Area (픽셀 단위)
*/
struct UISafeArea
{
    float Left{ 0 };
    float Right{ 0 };
    float Top{ 0 };
    float Bottom{ 0 };

    bool operator==(const UISafeArea& rhs) const
    {
        return Left == rhs.Left &&
            Right == rhs.Right &&
            Top == rhs.Top &&
            Bottom == rhs.Bottom;
    }

    bool operator!=(const UISafeArea& rhs) const
    {
        return !(*this == rhs);
    }
};

/*
    UISpaceContext
    -------------------------------------------------
    - Screen UI 계산에 필요한 모든 외부 상태
    - 계산 로직 없음 (pure state container)
    - 변경 여부만 반환하여 상위에서 Dirty 전파 판단
*/
class UISpaceContext
{
public:
    // =====================================================
    // State setters (Dirty-aware)
    // =====================================================

    bool SetViewportSize(uint32 w, uint32 h)
    {
        if (ViewportW == w && ViewportH == h)
            return false;

        ViewportW = w;
        ViewportH = h;
        return true;
    }

    bool SetDPIScale(float dpi)
    {
        if (DPI == dpi)
            return false;

        DPI = dpi;
        return true;
    }

    bool SetSafeArea(const UISafeArea& area)
    {
        if (SafeArea == area)
            return false;

        SafeArea = area;
        return true;
    }

public:
    // =====================================================
    // Public state (read-only usage 권장)
    // =====================================================

    uint32 ViewportW{ 0 };
    uint32 ViewportH{ 0 };

    float  DPI{ 1.0f };

    UISafeArea SafeArea{};
};