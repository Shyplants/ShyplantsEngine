#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/UIActor.h"
#include "Engine/Core/Component/SceneComponent.h"

UIActor::UIActor()
{
	// UIActor는 SceneComponent를 갖지만,
	// World 변환이 아닌 Screen 좌표 용도로 사용됨.
	m_rootComponent = AddComponent<SceneComponent>();
	m_rootComponent->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
}

UIActor::~UIActor()
{
}

void UIActor::OnSpawned()
{
	Actor::OnSpawned();
}

void UIActor::BeginPlay()
{
	Actor::BeginPlay();
}
