#pragma once

#include <memory>

class Level;
class Actor;
class D3D11Renderer;
class CameraComponent;

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

	// Camera Management
	void SetActiveCamera(CameraComponent* camera) { m_activeCamera = camera; }
	CameraComponent* GetActiveCamera() const { return m_activeCamera; }
	bool HasActiveCamera() const { return m_activeCamera != nullptr; }
private:
	Actor* SpawnActor_Impl(std::unique_ptr<Actor> actor);

private:
	std::unique_ptr<Level> m_currentLevel{ nullptr };

	CameraComponent* m_activeCamera{ nullptr };
};