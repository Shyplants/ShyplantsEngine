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
    - 멀티/싱글 플레이 공통 베이스
    - 생성자에서는 가상 함수 호출 금지
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
    // Initialization
    // =====================================================
    void Initialize();   // GameMode 세팅 완료 후 호출

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
    // Factory hooks (derived GameMode override)
    // =====================================================
    virtual std::unique_ptr<GameState> CreateGameState();
    virtual std::unique_ptr<PlayerController> CreatePlayerController();
    virtual std::unique_ptr<PlayerState> CreatePlayerState();

protected:
    World& m_world;

    std::unique_ptr<GameState>        m_gameState;
    std::unique_ptr<PlayerController> m_playerController;

private:
    bool m_initialized{ false };
    bool m_hasBegunPlay{ false };
};
