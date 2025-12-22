#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Common/Types.h"

// Forward declarations
class Actor;
class World;
class RenderQueue;
class CameraComponent2D;

/*
    Level
    -------------------------------------------------
    - Gameplay space / scene
    - Owns Actors
    - Collects DrawCommands and submits to RenderQueue
    - Does NOT execute rendering
*/
class Level
{
public:
    Level() = default;
    virtual ~Level();

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

public:
    // =====================================================
    // Lifecycle (called by World)
    // =====================================================
    virtual void OnEnter(World& world);
    virtual void OnExit(World& world);

    virtual void OnBeginPlay();
    virtual void Tick(float deltaTime);

public:
    // =====================================================
    // Rendering (Submit phase only)
    // =====================================================
    void SubmitRenderCommands(
        RenderQueue& queue,
        const CameraComponent2D& camera);

public:
    // =====================================================
    // Actor Management
    // =====================================================
    Actor* SpawnActorInternal(std::unique_ptr<Actor> actor);
    void   MarkActorForDestroy(Actor* actor);

    const std::vector<std::unique_ptr<Actor>>&
        GetActors() const { return m_actors; }

protected:
    World& GetWorld() const;

private:
    void ProcessDestroyedActors();

    void SubmitWorldRenderers(
        RenderQueue& queue,
        const CameraComponent2D& camera);

    void SubmitUIRenderers(
        RenderQueue& queue);

private:
    World* m_world{ nullptr };

    std::vector<std::unique_ptr<Actor>> m_actors;
    std::vector<Actor*> m_destroyQueue;

    bool m_hasBegunPlay{ false };
};
