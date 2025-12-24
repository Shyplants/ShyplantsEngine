#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Common/Types.h"
#include "Engine/Core/World/LevelTypes.h"

// Forward declarations
class Actor;
class World;
class RenderQueue;
class CameraComponent2D;
class RendererComponent;

/*
    Level
    -------------------------------------------------
    - Owns Actors
    - Actor lifecycle container
    - Persistent / Gameplay Level 공통 베이스
*/
class Level
{
public:
    Level();
    virtual ~Level();

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

public:
    // =====================================================
    // Identity
    // =====================================================
    virtual ELevelType GetLevelType() const = 0;

public:
    // =====================================================
    // Lifecycle (called by World)
    // =====================================================
    virtual void OnEnter(World& world);
    virtual void OnExit(World& world);

    virtual void OnBeginPlay();
    virtual void Tick(float deltaTime);

    // World 종료 시 호출
    virtual void Shutdown();

public:
    // =====================================================
    // Rendering (Submit-only)
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

protected:
    World& GetWorld() const;

private:
    // =====================================================
    // Internals
    // =====================================================
    void ProcessDestroyedActors();
    void DestroyAllActors();

    void SubmitWorldRenderers(
        RenderQueue& queue,
        const CameraComponent2D& camera);

    void SubmitUIRenderers(RenderQueue& queue);

private:
    World* m_world{ nullptr };

    std::vector<std::unique_ptr<Actor>> m_actors;
    std::vector<Actor*> m_destroyQueue;

    bool m_hasBegunPlay{ false };
    bool m_isShuttingDown{ false };
};
