#pragma once

#include "Common/Types.h"

class World;

class GameState
{
public:
	explicit GameState(World* world);
	virtual ~GameState();

	virtual void OnBeginPlay() {}
	virtual void Tick(float deltaTime);

public:
	void SetScore(int32 score) { m_score = score; }
	int32 GetScore() const { return m_score; }

	void AddScore(int delta) { m_score += delta; }

protected:
	World* GetWorld() const { return m_world; }

private:
	World* m_world{ nullptr };

	int32 m_score{ 0 };
	float m_elapsedTime{ 0.0f };
	bool m_isGameOver{ false };
};