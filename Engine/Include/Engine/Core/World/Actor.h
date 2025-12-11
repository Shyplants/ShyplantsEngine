#pragma once

#include "Engine/Core/Component/FAttachmentTransformRules.h"
#include <vector>
#include <memory>

class ActorComponent;
class SceneComponent;
class RendererComponent;
class World;

class Actor
{
public:
	Actor();
	virtual ~Actor();

	// 라이프 사이클
	virtual void OnSpawned() {}
	virtual void BeginPlay() {}
	virtual void Tick(float deltaTime);
	virtual void Destroy();

	bool IsPendingDestroy() const { return m_pendingDestroy; }
	virtual bool IsUIActor() const { return false; }

	

	// 컴포넌트 시스템
	template <typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of_v<ActorComponent, T>, "T must derive from ActorComponent");

		auto comp = std::make_unique<T>(this, std::forward<Args>(args)...);
		T* rawPtr = comp.get();

		comp->SetWorld(m_world);

		// SceneComponent 파생 컴포넌트
		if constexpr (std::is_base_of_v<SceneComponent, T>)
		{
			SceneComponent* sceneComp = static_cast<SceneComponent*>(rawPtr);

			if (m_rootComponent && sceneComp != m_rootComponent)
			{
				sceneComp->AttachTo(m_rootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			}
		}

		// RendererComponent 파생 컴포넌트
		else if constexpr (std::is_base_of_v<RendererComponent, T>)
		{
			RendererComponent* renderComp = static_cast<RendererComponent*>(rawPtr);
			if(m_rootComponent)
				renderComp->SetAttachComponent(m_rootComponent);
		}

		m_components.push_back(std::move(comp));
		rawPtr->OnRegister();
		
		return rawPtr;
	}

	template <typename T>
	T* GetComponent() const
	{
		for (auto& comp : m_components)
		{
			if (auto casted = dynamic_cast<T*>(comp.get()))
				return casted;
		}

		return nullptr;
	}

	// RootComponent
	SceneComponent* GetRootComponent() const { return m_rootComponent; }
	void SetRootComponent(SceneComponent* newRoot);

	// Actor hierarchy
	Actor* GetParentActor() const { return m_parentActor; }
	const std::vector<Actor*>& GetChildActors() const { return m_childActors; }

	void AttachToActor(Actor* parent, const FAttachmentTransformRules& rules = FAttachmentTransformRules::KeepWorldTransform);
	void DetachFromParent(const FDetachmentTransformRules& rules = FDetachmentTransformRules::KeepWorldTransform);

	// World
	World* GetWorld() const { return m_world; }
	void SetWorld(World* world);

	// Component
	const std::vector<std::unique_ptr<ActorComponent>>& GetComponents() const { return m_components; }

protected:
	SceneComponent* m_rootComponent = nullptr;

private:
	std::vector<std::unique_ptr<ActorComponent>> m_components{};
	std::vector<Actor*> m_childActors;

	Actor* m_parentActor{ nullptr };
	World* m_world{ nullptr };
	
	bool m_pendingDestroy{ false };	
};