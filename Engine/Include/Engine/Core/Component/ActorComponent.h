#pragma once

#include <memory>

class Actor;
class World;

class ActorComponent
{
public:
	explicit ActorComponent(Actor* owner);
	virtual ~ActorComponent();
	
	// 라이프 사이클
	virtual void OnRegister() {}
	virtual void OnUnregister() {}
	virtual void OnDestroy() {}
	virtual void Tick(float deltaTime) {}

	// 상태
	void SetActive(bool active) { m_active = active; }
	bool IsActive() const { return m_active; }

	bool IsTickEnabled() const { return m_canTick; }
	void SetTickEnabled(bool enabled) { m_canTick = enabled; }

	// 소유 Actor/World
	Actor* GetOwner() const { return m_owner; }
	World* GetWorld() const;
	
	// 내부적으로 World 설정 (Actor가 호출)
	void SetWorld(World* world) { m_world = world; }

protected:
	Actor* m_owner{ nullptr };
	World* m_world{ nullptr };

	bool m_active{ true };
	bool m_canTick{ true };
};