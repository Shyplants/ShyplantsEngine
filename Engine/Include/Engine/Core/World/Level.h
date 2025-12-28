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

    // Level + GameMode가 모두 준비된 후 호출
    virtual void OnGamePlayStart();

    // World 종료 시 호출
    virtual void Shutdown();

public:
    // =====================================================
    // Rendering
    // =====================================================
    virtual void SubmitRenderCommands(
        RenderQueue& queue,
        CameraComponent2D& activeCamera);

public:
    // =====================================================
    // Actor Management
    // =====================================================
    Actor* SpawnActorInternal(std::unique_ptr<Actor> actor);
    void   MarkActorForDestroy(Actor* actor);

    // =====================================================
    // Actor Query
    // =====================================================
    template<typename T>
    T* FindActor() const
    {
        for (const auto& actor : m_actors)
        {
            if (!actor)
                continue;

            if (auto* casted = dynamic_cast<T*>(actor.get()))
                return casted;
        }
        return nullptr;
    }

    template<typename T>
    std::vector<T*> FindActors() const
    {
        std::vector<T*> result;
        for (const auto& actor : m_actors)
        {
            if (!actor)
                continue;

            if (auto* casted = dynamic_cast<T*>(actor.get()))
                result.push_back(casted);
        }
        return result;
    }

protected:
    World& GetWorld() const;

private:
    // =====================================================
    // Internals
    // =====================================================
    void ProcessDestroyedActors();
    void DestroyAllActors();

private:
    World* m_world{ nullptr };

    std::vector<std::unique_ptr<Actor>> m_actors;
    std::vector<Actor*> m_destroyQueue;

    bool m_hasBegunPlay{ false };
    bool m_isShuttingDown{ false };
};
