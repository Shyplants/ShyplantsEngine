#pragma once

#include "Engine/Resource/Texture/TextureColorSpace.h"

/*
    TextureDesc
    -------------------------------------------------
    - TextureResource 로딩 옵션 묶음
    - Builder를 통해 설정됨
*/
struct TextureDesc
{
    TextureColorSpace colorSpace = TextureColorSpace::Linear;
};
