#pragma once

#include "Engine/Core/World/WorldActor.h"

class SpriteRendererComponent;

class DemoActor : public WorldActor
{
public:
    DemoActor();
    ~DemoActor() override = default;

    void OnSpawned() override;
    void OnBeginPlay() override;

private:
    SpriteRendererComponent* m_spriteRenderer = nullptr;
};
