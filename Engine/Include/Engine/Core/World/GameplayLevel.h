#pragma once

#include "Engine/Core/World/Level.h"

/*
    GameplayLevel
    -------------------------------------------------
    - 맵 / 스테이지 단위 Level
    - 교체 가능한 Gameplay Actor 소유
*/
class GameplayLevel : public Level
{
public:
    ELevelType GetLevelType() const override
    {
        return ELevelType::Gameplay;
    }
};
