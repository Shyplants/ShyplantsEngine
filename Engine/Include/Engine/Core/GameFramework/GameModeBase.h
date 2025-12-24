#pragma once

#include <memory>

// Forward declarations
class World;
class GameState;
class PlayerController;
class PlayerState;

/*
    GameModeBase
    -------------------------------------------------
    - 엔진 기본 게임 모드 (중립)
    - 멀티플레이/싱글플레이 모두 대응 가능
    - Pawn, Player 수에 대한 가정 없음
*/
class GameModeBase
{
public:
    explicit GameModeBase(World& world);
    virtual ~GameModeBase();

    GameModeBase(const GameModeBase&) = delete;
    GameModeBase& operator=(const GameModeBase&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    virtual void OnBeginPlay();
    virtual void Tick(float deltaTime);
    virtual void OnEndPlay();

public:
    // =====================================================
    // Accessors
    // =====================================================
    GameState* GetGameState() const { return m_gameState.get(); }
    PlayerController* GetPlayerController() const { return m_playerController.get(); }
    World& GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Factory hooks
    // =====================================================
    virtual std::unique_ptr<GameState> CreateGameState();
    virtual std::unique_ptr<PlayerController> CreatePlayerController();
    virtual std::unique_ptr<PlayerState> CreatePlayerState();

protected:
    World& m_world;

    std::unique_ptr<GameState>        m_gameState;
    std::unique_ptr<PlayerController> m_playerController;

private:
    bool m_hasBegunPlay{ false };
};
