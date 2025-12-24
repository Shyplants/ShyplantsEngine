#pragma once

#include "Engine/Core/World/GameplayLevel.h"

class World;
class DemoLevel final: public GameplayLevel
{
public:
	DemoLevel();
	~DemoLevel() override;

	void OnEnter(World& world) override;
	void OnBeginPlay() override;
};