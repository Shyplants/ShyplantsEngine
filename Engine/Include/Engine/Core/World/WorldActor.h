#pragma once

#include "Engine/Core/World/Actor.h"

// Forward declarations
class WorldTransformComponent;

/*
    WorldActor
    -------------------------------------------------
    - World-space Actor
    - Gameplay °´Ã¼
*/
class WorldActor : public Actor
{
public:
    WorldActor();
    ~WorldActor() override;

public:
    bool IsUIActor() const override { return false; }

    void OnSpawned() override;

public:
    WorldTransformComponent* GetWorldTransform() const
    {
        return m_worldTransform;
    }

private:
    WorldTransformComponent* m_worldTransform{ nullptr };
};
