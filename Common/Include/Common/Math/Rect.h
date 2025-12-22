#pragma once

#include "Common/Types.h"

/*
    Rect
    -------------------------------------------------
    - 엔진 공용 2D 정수 사각형 타입
    - 플랫폼 독립 (Win32 RECT 대체)
    - UI / Sprite / Atlas / Scissor 등 공용 사용
*/
struct Rect
{
    int32 left{ 0 };
    int32 top{ 0 };
    int32 right{ 0 };
    int32 bottom{ 0 };

    constexpr Rect() = default;

    constexpr Rect(
        int32 l,
        int32 t,
        int32 r,
        int32 b)
        : left(l), top(t), right(r), bottom(b)
    {
    }

    // ============================
    // Size
    // ============================

    constexpr int32 Width() const
    {
        return right - left;
    }

    constexpr int32 Height() const
    {
        return bottom - top;
    }

    constexpr bool IsEmpty() const
    {
        return Width() <= 0 || Height() <= 0;
    }

    // ============================
    // Utilities
    // ============================

    constexpr bool Contains(int32 x, int32 y) const
    {
        return x >= left && x < right
            && y >= top && y < bottom;
    }

    constexpr bool Intersects(const Rect& other) const
    {
        return !(other.right <= left
            || other.left >= right
            || other.bottom <= top
            || other.top >= bottom);
    }

    // ============================
    // Static Helpers
    // ============================

    static constexpr Rect FromPositionSize(
        int32 x,
        int32 y,
        int32 width,
        int32 height)
    {
        return Rect(x, y, x + width, y + height);
    }
};
