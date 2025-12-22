#pragma once

#include "Engine/Core/World/Level.h"

class World;
class DemoLevel : public Level
{
public:
	DemoLevel();
	~DemoLevel() override;

	void OnEnter(World& world) override;
	void OnBeginPlay() override;
};