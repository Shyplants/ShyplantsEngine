#pragma once

#include "Engine/Core/World/Level.h"

/*
    PersistentLevel
    -------------------------------------------------
    - World lifetime 동안 유지되는 Level
    - Player, Camera, UI 등 영속 Actor 소유
*/
class PersistentLevel final : public Level
{
public:
    ELevelType GetLevelType() const override
    {
        return ELevelType::Persistent;
    }
};
