#pragma once

#include <memory>

class World;
class GameState;

class GameMode
{
public:
	explicit GameMode(World* world);
	virtual ~GameMode();

	virtual void OnBeginPlay();
	
	virtual void Tick(float deltaTime);

public:
	GameState* GetGameState() const { return m_gameState.get(); }

	World* GetWorld() const { return m_world; }

protected:
	virtual std::unique_ptr<GameState> CreateGameState();

protected:
	World* m_world{ nullptr };
	std::unique_ptr<GameState> m_gameState{ nullptr };
};