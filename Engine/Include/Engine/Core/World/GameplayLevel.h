#pragma once

#include "Engine/Core/World/Level.h"

class LevelResourceSet;

/*
    GameplayLevel
    -------------------------------------------------
    - 교체 가능한 Gameplay Level
    - Level 전용 리소스 세트 소유
*/
class GameplayLevel : public Level
{
public:
    GameplayLevel();
    ~GameplayLevel() override;

public:
    ELevelType GetLevelType() const override
    {
        return ELevelType::Gameplay;
    }

public:
    void OnEnter(World& world) override;
    void OnExit(World& world) override;
    void Shutdown() override;

protected:
    void SetResourceSet(std::unique_ptr<LevelResourceSet> resources);
    LevelResourceSet* GetResourceSet() const;

private:
    std::unique_ptr<LevelResourceSet> m_resources;
};
