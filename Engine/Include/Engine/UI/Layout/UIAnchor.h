#pragma once

#include "Common/Types.h"

/*
    UIAnchor
    -------------------------------------------------
    - Screen space 기준 Anchor 정의
    - Pivot이 아닌 "기준 영역 선택자"
*/
enum class EUIAnchor : uint8
{
    Center,

    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight,
};

/*
    UIAnchorData
    -------------------------------------------------
    - Anchor + Offset 조합
*/
struct UIAnchorData
{
    EUIAnchor Anchor{ EUIAnchor::Center };
    float OffsetX{ 0.0f };
    float OffsetY{ 0.0f };
};
