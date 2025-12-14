#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/Component/RendererComponent.h"

Actor::Actor()
{
	m_rootComponent = AddComponent<SceneComponent>();
	SetRootComponent(m_rootComponent);
}

Actor::~Actor() = default;

void Actor::Tick(float deltaTime)
{
	for (auto& comp : m_components)
	{
		comp->Tick(deltaTime);
	}
}

void Actor::Destroy()
{
	if (m_pendingDestroy)
		return;

	m_pendingDestroy = true;

	for (Actor* child : m_childActors)
	{
		if (child && !child->IsPendingDestroy())
			child->Destroy();
	}

	if (m_world)
		m_world->DestroyActor(this);
}

void Actor::SetWorld(World* world)
{
	m_world = world;

	for (auto& comp : m_components)
	{
		comp->SetWorld(world);
	}
}

void Actor::SetRootComponent(SceneComponent* newRoot)
{
	if (!newRoot || newRoot == m_rootComponent)
		return;

	SceneComponent* oldRoot = m_rootComponent;
	m_rootComponent = newRoot;

	if (oldRoot)
	{
		auto children = oldRoot->GetChildren();
		for (SceneComponent* child : children)
		{
			child->Detach(FDetachmentTransformRules::KeepWorldTransform);
			child->AttachTo(newRoot, FAttachmentTransformRules::KeepWorldTransform);
		}
	}

	if (m_rootComponent->GetParent())
		m_rootComponent->Detach(FDetachmentTransformRules::KeepWorldTransform);
}

void Actor::AttachToActor(Actor* parent, const FAttachmentTransformRules& rules)
{
	if (!parent || parent == this)
		return;

	if (m_parentActor)
		DetachFromParent();

	m_parentActor = parent;
	parent->m_childActors.push_back(this);

	// RootComponent³¢¸® Attach
	if (m_rootComponent && parent->m_rootComponent)
	{
		m_rootComponent->AttachTo(parent->m_rootComponent, rules);
	}
}

void Actor::DetachFromParent(const FDetachmentTransformRules& rules)
{
	if (!m_parentActor)
		return;

	auto& siblings = m_parentActor->m_childActors;
	siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());

	if (m_rootComponent)
		m_rootComponent->Detach(rules);

	m_parentActor = nullptr;
}