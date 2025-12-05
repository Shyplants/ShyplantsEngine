#pragma once

#include "Common/Types.h"

enum class GameLayer : uint32_t
{
    Background = 0,
    TileMap = 50,
    WorldObject = 100,
    Player = 150,
    Effects = 200,
    UI_Background = 900,
    UI = 1000,
    UI_Front = 1100,
};