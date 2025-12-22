#pragma once

#include <memory>
#include <type_traits>

#include "Common/Types.h"

// Forward declarations
class Level;
class Actor;
class RenderSystem;
class RenderQueue;

class CameraComponent2D;
class GameMode;
class GameState;

/*
    World
    -------------------------------------------------
    - Gameplay state container
    - Owns current Level
    - Manages Actor lifecycle
    - Manages active camera
    - Owns render submit scheduling (Submit phase)
*/
class World
{
public:
    explicit World(RenderSystem* renderSystem);
    ~World();

    World(const World&) = delete;
    World& operator=(const World&) = delete;

public:
    // =====================================================
    // Tick (Update Phase)
    // =====================================================
    void Tick(float deltaTime);

public:
    // =====================================================
    // Render Submit Phase
    // =====================================================
    void SubmitRenderCommands();

public:
    // =====================================================
    // Actor Management
    // =====================================================
    template<typename T, typename... Args>
    T* SpawnActor(Args&&... args)
    {
        static_assert(std::is_base_of_v<Actor, T>,
            "T must be derived from Actor");

        auto actor = std::make_unique<T>(
            std::forward<Args>(args)...);

        return static_cast<T*>(
            SpawnActor_Impl(std::move(actor)));
    }

    void DestroyActor(Actor* actor);

public:
    // =====================================================
    // Level Management
    // =====================================================
    void LoadLevel(std::unique_ptr<Level> level);
    void UnloadCurrentLevel();

    Level* GetCurrentLevel() const { return m_currentLevel.get(); }

public:
    // =====================================================
    // Game Framework
    // =====================================================
    template<typename T>
    T* GetGameMode() const
    {
        return dynamic_cast<T*>(m_gameMode.get());
    }

    GameState* GetGameState() const;
    void SetGameMode(std::unique_ptr<GameMode> gameMode);

public:
    // =====================================================
    // Camera
    // =====================================================
    void SetActiveCamera(CameraComponent2D* camera);
    CameraComponent2D* GetActiveCamera() const { return m_activeCamera; }

    void OnViewportResized(uint32 width, uint32 height);

    // Called by Level when an actor is being destroyed
    void NotifyActorDestroyed(Actor* actor);

public:
    // =====================================================
    // Rendering (low-level access)
    // =====================================================
    RenderQueue& GetRenderQueue() const;

    RenderSystem& GetRenderSystem() const;

private:
    // =====================================================
    // Internal
    // =====================================================
    void CreateGameFramework();
    Actor* SpawnActor_Impl(std::unique_ptr<Actor> actor);

private:
    RenderSystem* m_renderSystem{ nullptr };

    std::unique_ptr<Level>    m_currentLevel;
    std::unique_ptr<GameMode> m_gameMode;

    CameraComponent2D* m_activeCamera{ nullptr };

    uint32 m_lastViewportW{ 0 };
    uint32 m_lastViewportH{ 0 };
};
