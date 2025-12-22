#pragma once

#include "Engine/Core/World/Actor.h"

class SpriteRendererComponent;
class Material;
class SpriteAnimatorComponent;

class DemoAnimationActor : public Actor
{
public:
	DemoAnimationActor();
	~DemoAnimationActor() override = default;

	void OnSpawned() override;
	void Tick(float deltaTime) override;

private:
	SpriteRendererComponent* m_sprite{ nullptr };
	SpriteAnimatorComponent* m_animator{ nullptr };
};