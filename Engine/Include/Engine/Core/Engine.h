#pragma once

#include <memory>

#include "Common/Types.h"
#include "Engine/Core/Time/TimeSystem.h"

// Forward declarations
class Window;
class GraphicsSubsystem;
class World;
class GameModeBase;
class GameplayLevel;

/*
    Engine
    -------------------------------------------------
    - ShyplantsEngine의 최상위 루트 객체
    - Subsystem / World 수명 관리
    - Main loop (Tick / Render)
    - Client 친화적 파사드 제공
*/
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

public:
    // =========================================================
    // Lifecycle
    // =========================================================
    bool Initialize(Window& window);
    void Shutdown();

public:
    // =========================================================
    // Main Loop
    // =========================================================
    void Tick();

public:
    // =========================================================
    // Resize
    // =========================================================
    void Resize(uint32 width, uint32 height);

public:
    // =========================================================
    // Game Setup (Facade)
    // =========================================================
    void SetGameMode(std::unique_ptr<GameModeBase> gameMode);
    void LoadGameplayLevel(std::unique_ptr<GameplayLevel> level);

public:
    // =========================================================
    // Accessors
    // =========================================================
    GraphicsSubsystem& GetGraphics();
    World& GetWorld();

private:
    // =========================================================
    // Subsystems
    // =========================================================
    std::unique_ptr<GraphicsSubsystem> m_graphics;

private:
    // =========================================================
    // Game World
    // =========================================================
    std::unique_ptr<World> m_world;

private:
    TimeSystem m_time;
    bool m_initialized{ false };
};
