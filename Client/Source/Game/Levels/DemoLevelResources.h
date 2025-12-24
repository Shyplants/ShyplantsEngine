#pragma once

#include "Engine/Core/World/LevelResourceSet.h"

class DemoLevelResources : public LevelResourceSet
{
public:
    ~DemoLevelResources() override;

    void Load() override;
    void Unload() override;
};