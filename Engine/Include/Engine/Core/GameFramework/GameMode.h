#pragma once

#include <memory>

// Forward declarations
class World;
class GameState;
class PlayerController;
class Pawn;

/*
    GameMode
    -------------------------------------------------
    - World 단위의 게임 규칙 컨트롤러
    - PlayerController / Pawn / GameState 수명 관리
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
    virtual void OnBeginPlay();
    virtual void Tick(float deltaTime);
    virtual void OnEndPlay();

public:
    // =====================================================
    // Accessors
    // =====================================================
    GameState* GetGameState() const { return m_gameState.get(); }
    PlayerController* GetPlayerController() const { return m_playerController.get(); }
    Pawn* GetPawn() const { return m_pawn; }
    World& GetWorld() const { return m_world; }

protected:
    // =====================================================
    // Factories (override point)
    // =====================================================
    virtual std::unique_ptr<GameState>        CreateGameState();
    virtual std::unique_ptr<PlayerController> CreatePlayerController();
    virtual Pawn* SpawnDefaultPawn();

protected:
    World& m_world;

    std::unique_ptr<GameState>        m_gameState;
    std::unique_ptr<PlayerController> m_playerController;

    Pawn* m_pawn{ nullptr };

private:
    bool m_hasBegunPlay{ false };
};
