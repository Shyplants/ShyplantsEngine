#pragma once

#include "Engine/Core/World/Level.h"

class World;
class DemoLevel : public Level
{
public:
	explicit DemoLevel(World* world);
	~DemoLevel() override;

	void OnBeginPlay() override;
	void Tick(float deltaTime) override;
};