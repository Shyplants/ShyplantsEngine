#pragma once

#include <memory>

class Level;
class Actor;
class D3D11Renderer;
// class CameraComponent;
class CameraComponent2D;
class GameMode;
class GameState;

class World
{
public:
	World();
	~World();

	void Tick(float deltaTime);
	void Render(D3D11Renderer& renderer);

	// Actor Management
	template<typename T, typename... Args>
	T* SpawnActor(Args&&... args)
	{
		static_assert(std::is_base_of_v<Actor, T>, "T must be derived from Actor");

		auto actor = std::make_unique<T>(std::forward<Args>(args)...);
		return static_cast<T*>(SpawnActor_Impl(std::move(actor)));
	}

	void DestroyActor(Actor* actor);

	// Level Management
	void LoadLevel(std::unique_ptr<Level> level);
	void UnloadCurrentLevel();

	Level* GetCurrentLevel() const { return m_currentLevel.get(); }

	// GameMode/GameState Management
	GameMode* GetGameMode() const { return m_gameMode.get(); }
	GameState* GetGameState() const;

	// Camera Management
	CameraComponent2D* GetMainCamera() const { return m_mainCamera; }
	void SetMainCamera(CameraComponent2D* camera);

private:
	void CreateGameFramework();
	
private:
	Actor* SpawnActor_Impl(std::unique_ptr<Actor> actor);

private:
	std::unique_ptr<Level> m_currentLevel{ nullptr };
	std::unique_ptr<GameMode> m_gameMode;

	CameraComponent2D* m_mainCamera{ nullptr };
};