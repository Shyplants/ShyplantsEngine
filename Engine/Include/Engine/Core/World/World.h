#pragma once

#include <memory>
#include <type_traits>

#include "Common/Types.h"

// Forward declarations
class Actor;
class Level;
class PersistentLevel;
class GameplayLevel;

class RenderSystem;
class RenderQueue;

class CameraComponent2D;
class GameModeBase;
class GameState;

/*
    World
    -------------------------------------------------
    - 엔진의 최상위 Gameplay 컨테이너
    - PersistentLevel + GameplayLevel 소유
    - GameModeBase 생명주기 관리
    - 정책 없는 오케스트레이터
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

    // Level + GameMode 준비 완료 후 호출
    void StartGameplay();

    bool IsShuttingDown() const { return m_isShuttingDown; }

public:
    // =====================================================
    // Render Submit
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
            SpawnActor_Internal(std::move(actor)));
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
    void SetGameMode(std::unique_ptr<GameModeBase> gameMode);

    GameModeBase* GetGameMode() const;

    template<typename T>
    T* GetGameModeAs() const
    {
        return static_cast<T*>(GetGameMode());
    }

    GameState* GetGameState() const;

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
    // Rendering Access
    // =====================================================
    RenderQueue& GetRenderQueue() const;
    RenderSystem& GetRenderSystem() const;

private:
    // =====================================================
    // Internal
    // =====================================================
    Actor* SpawnActor_Internal(std::unique_ptr<Actor> actor);

private:
    RenderSystem* m_renderSystem{ nullptr };

    std::unique_ptr<PersistentLevel> m_persistentLevel;
    std::unique_ptr<GameplayLevel>   m_gameplayLevel;

    std::unique_ptr<GameModeBase> m_gameMode;

    CameraComponent2D* m_activeCamera{ nullptr };

    uint32 m_lastViewportW{ 0 };
    uint32 m_lastViewportH{ 0 };

    bool m_isShuttingDown{ false };
};
