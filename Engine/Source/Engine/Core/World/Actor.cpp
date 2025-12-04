#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/SceneComponent.h"

Actor::Actor()
{
	m_rootComponent = AddComponent<SceneComponent>(this);
}

Actor::~Actor()
{
	if (m_rootComponent)
	{
		m_rootComponent->Detach();
		m_rootComponent = nullptr;
	}
}

void Actor::Tick(float deltaTime)
{
	for (auto& comp : m_components)
	{
		comp->Tick(deltaTime);
	}
}

void Actor::SetRootComponent(SceneComponent* newRoot)
{
	if (newRoot == nullptr)
	{
		// RootComponent가 nullptr이 되는 것은 허용하지 않음
		// 모든 Actor는 RootComponent를 가져야 함 (엔진 규칙)
		return;
	}

	if (m_rootComponent == newRoot)
		return;

	SceneComponent* oldRoot = m_rootComponent;

	if (oldRoot)
		oldRoot->Detach();

	// 기존 RootComponent의 자식들을 새 Root에 Attach
	if (oldRoot)
	{
		auto children = oldRoot->GetChildren();
		for (SceneComponent* child : children)
		{
			child->Detach();
			child->AttachTo(newRoot);
		}
	}

	m_rootComponent = newRoot;

	if (newRoot->GetParent())
		newRoot->Detach();

	if (m_rootComponent)
		m_rootComponent->Detach();

	
	// 기존 RootComponent는 새 Root 아래로 이동
	if (oldRoot)
		oldRoot->AttachTo(newRoot);
}
