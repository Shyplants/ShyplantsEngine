#pragma once

#include "Engine/UI/UIActor.h"

class SpriteRendererComponent;

class DemoUIActor : public UIActor
{
public:
	DemoUIActor();
	~DemoUIActor() override;

	void OnSpawned() override;

private:
	SpriteRendererComponent* m_spriteRenderer = nullptr;
	
};