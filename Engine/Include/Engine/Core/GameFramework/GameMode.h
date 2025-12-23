#pragma once

#include <memory>

class World;
class GameState;

/*
    GameMode
    -------------------------------------------------
    - World 단위의 게임 규칙 컨트롤러
    - GameState를 생성/소유
    - Gameplay Session의 시작과 종료를 관리
*/
class GameMode
{
public:
    explicit GameMode(World& world);
    virtual ~GameMode();

    GameMode(const GameMode&) = delete;
    GameMode& operator=(const GameMode&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================

    // Called once when gameplay session begins
    virtual void OnBeginPlay();

    // Called every frame during gameplay session
    virtual void Tick(float deltaTime);

    // Called once when gameplay session ends
    // NOTE:
    // - Must NOT destroy actors
    // - Must NOT access RenderSystem
    // - Only clean up GameMode-owned logic/state
    virtual void OnEndPlay();

public:
    // =====================================================
    // Accessors
    // =====================================================
    GameState* GetGameState() const { return m_gameState.get(); }
    World& GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Factory
    // =====================================================
    virtual std::unique_ptr<GameState> CreateGameState();

protected:
    World& m_world;
    std::unique_ptr<GameState> m_gameState{ nullptr };

private:
    bool m_hasBegunPlay{ false };
};
