#pragma once

#include <memory>

#include "Common/Types.h"
#include "Engine/Core/Time/TimeSystem.h"

// Forward declarations
class Window;
class GraphicsSubsystem;
class World;

/*
    Engine
    -------------------------------------------------
    - ShyplantsEngine의 최상위 루트 객체
    - Subsystem / ResourceManager 생성 및 수명 관리
    - Main loop (Tick / Render) 담당
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
    // Game world
    // =========================================================
    std::unique_ptr<World> m_world;

private:
    TimeSystem m_time;

private:
    bool m_initialized{ false };
};
