#include "Engine/UI/Layout/UIAnchorResolver.h"

using namespace DirectX;

XMFLOAT2 UIAnchorResolver::Resolve(
    const UIAnchorData& a,
    const UISpaceContext& s)
{
    float left = s.SafeArea.Left;
    float right = s.ViewportW - s.SafeArea.Right;
    float top = s.ViewportH - s.SafeArea.Top;
    float bottom = s.SafeArea.Bottom;

    float x = 0.f, y = 0.f;

    switch (a.Anchor)
    {
    case EUIAnchor::Center:
        x = (left + right) * 0.5f;
        y = (top + bottom) * 0.5f;
        break;

    case EUIAnchor::TopLeft:     x = left;  y = top;    break;
    case EUIAnchor::TopCenter:   x = (left + right) * 0.5f; y = top; break;
    case EUIAnchor::TopRight:    x = right; y = top;    break;

    case EUIAnchor::MiddleLeft:  x = left;  y = (top + bottom) * 0.5f; break;
    case EUIAnchor::MiddleRight: x = right; y = (top + bottom) * 0.5f; break;

    case EUIAnchor::BottomLeft:  x = left;  y = bottom; break;
    case EUIAnchor::BottomCenter:x = (left + right) * 0.5f; y = bottom; break;
    case EUIAnchor::BottomRight: x = right; y = bottom; break;
    }

    return {
        x + a.OffsetX,
        y + a.OffsetY
    };
}
