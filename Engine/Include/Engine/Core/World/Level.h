#pragma once

#include <vector>
#include <memory>

class Actor;
class World;
class CameraComponent;
class D3D11Renderer;

class Level
{
public:
	explicit Level(World* world);
	virtual ~Level();

	virtual void OnBeginPlay();
	virtual void Tick(float deltaTime);

public:
	void Render(D3D11Renderer& renderer, const CameraComponent* camera);

	// Actor Management
	Actor* SpawnActorInternal(std::unique_ptr<Actor> actor);
	void MarkActorForDestroy(Actor* actor);

	// Helpers
	const std::vector<std::unique_ptr<Actor>>& GetActors() const { return m_actors; }
	std::vector<std::unique_ptr<Actor>>& GetActorsMutable() { return m_actors; }

protected:
	void ProcessDestroyedActors();
	World* GetWorld() const { return m_world; }

private:
	World* m_world = nullptr;

	std::vector<std::unique_ptr<Actor>> m_actors;
	std::vector<Actor*> m_destroyQueue;

	bool m_hasBegunPlay = false;
};