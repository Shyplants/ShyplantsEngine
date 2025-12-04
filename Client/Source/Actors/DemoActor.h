#pragma once

#include "Engine/Core/World/Actor.h"

class SpriteRendererComponent;

class DemoActor : public Actor
{
public:
	DemoActor();
	~DemoActor() override;

	void OnSpawned() override;
	void BeginPlay() override;

private:
	SpriteRendererComponent* m_sprite = nullptr;
};