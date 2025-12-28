#pragma once

#include "Common/Types.h"

/*
    ERenderSpace
    -------------------------------------------------
    - 렌더링 좌표 공간의 기준
    - World  : 카메라(View/Proj) 영향을 받는 월드 좌표
    - Screen : 화면 좌표(UI), Anchor / DPI 기반
*/
enum class ERenderSpace : uint8
{
    World = 0,
    Screen = 1,
};

/*
    ERenderLayer
    -------------------------------------------------
    - 동일 RenderSpace 내에서의 논리적 레이어
    - 숫자가 낮을수록 먼저 렌더링됨
*/
enum class ERenderLayer : uint8
{
    Background = 0,
    Default = 1,
    Foreground = 2,

    Debug = 3,
};

/*
    RenderCategory
    -------------------------------------------------
    - 렌더링 "의도"를 명시적으로 표현하는 구조체
    - RenderSpace + RenderLayer 조합
    - RenderQueue 정렬 및 Submit 분기 기준
*/
struct RenderCategory
{
    ERenderSpace Space{ ERenderSpace::World };
    ERenderLayer Layer{ ERenderLayer::Default };

    constexpr RenderCategory() = default;

    constexpr RenderCategory(
        ERenderSpace space,
        ERenderLayer layer)
        : Space(space)
        , Layer(layer)
    {
    }

    // ----------------------------
    // Helpers
    // ----------------------------
    constexpr bool IsWorldSpace() const
    {
        return Space == ERenderSpace::World;
    }

    constexpr bool IsScreenSpace() const
    {
        return Space == ERenderSpace::Screen;
    }

    // ----------------------------
    // Comparison
    // ----------------------------
    constexpr bool operator==(const RenderCategory& rhs) const
    {
        return Space == rhs.Space && Layer == rhs.Layer;
    }

    constexpr bool operator!=(const RenderCategory& rhs) const
    {
        return !(*this == rhs);
    }
};
