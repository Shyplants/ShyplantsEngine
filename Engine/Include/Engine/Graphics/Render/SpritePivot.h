#pragma once

#include "Common/Types.h"

enum class SpritePivot : uint32
{
    Center = 0,
    
    TopLeft,
    TopCenter,
    TopRight,

    MiddleLeft,
    MiddleCenter, // Center¿Í µ¿ÀÏ
    MiddleRight,

    BottomLeft,
    BottomCenter,
    BottomRight,

    Custom,
};