#pragma once

#include <DirectXMath.h>
#include "Engine/UI/Layout/UIAnchor.h"
#include "Engine/UI/UISpaceContext.h"

/*
    UIAnchorResolver
    -------------------------------------------------
    - Anchor 기준점을 Screen-space 픽셀 좌표로 변환
*/
class UIAnchorResolver
{
public:
    static DirectX::XMFLOAT2 Resolve(
        const UIAnchorData& anchor,
        const UISpaceContext& space);
};
