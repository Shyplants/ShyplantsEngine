#pragma once

#include <memory>
#include <type_traits>

#include "Common/Types.h"

// Forward declarations
class Level;
class PersistentLevel;
class GameplayLevel;
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
    - Owns PersistentLevel + GameplayLevel
    - Manages high-level game lifecycle
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
    // Lifecycle
    // =====================================================
    void Tick(float deltaTime);
    void Shutdown();

    bool IsShuttingDown() const { return m_isShuttingDown; }

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

        if (m_isShuttingDown)
            return nullptr;

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
    void LoadGameplayLevel(std::unique_ptr<GameplayLevel> level);
    void ShutdownGameplayLevel();

    PersistentLevel* GetPersistentLevel() const { return m_persistentLevel.get(); }
    GameplayLevel* GetGameplayLevel()   const { return m_gameplayLevel.get(); }

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

    std::unique_ptr<PersistentLevel> m_persistentLevel;
    std::unique_ptr<GameplayLevel>   m_gameplayLevel;

    std::unique_ptr<GameMode> m_gameMode;

    CameraComponent2D* m_activeCamera{ nullptr };

    uint32 m_lastViewportW{ 0 };
    uint32 m_lastViewportH{ 0 };

    bool m_isShuttingDown{ false };
};
