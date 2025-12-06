#pragma once

#include "Engine/Core/World/Actor.h"

class SpriteRendererComponent;
class SpriteAnimatorComponent;

class DemoAnimationActor : public Actor
{
public:
	DemoAnimationActor();
	~DemoAnimationActor() override;

	void OnSpawned() override;
	void Tick(float deltaTime) override;

private:
	SpriteRendererComponent* m_renderer{ nullptr };
	SpriteAnimatorComponent* m_animator{ nullptr };
};