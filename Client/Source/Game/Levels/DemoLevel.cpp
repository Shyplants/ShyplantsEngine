#include "ClientPCH/ClientPCH.h"
#include "Game/Levels/DemoLevel.h"

#include "Game/Actors/DemoActor.h"
// #include "Game/Actors/DemoUIActor.h"
// #include "Game/Actors/DemoAnimationActor.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/SceneComponent.h"


DemoLevel::DemoLevel() = default;

DemoLevel::~DemoLevel() = default;

void DemoLevel::OnEnter(World& world)
{
	Level::OnEnter(world);
}

void DemoLevel::OnBeginPlay()
{
	Level::OnBeginPlay();

	SP_LOG(LogGame, ELogLevel::Info, "DemoLevel::OnBeginPlay called");

	auto& world = GetWorld();

	auto* demoActor = world.SpawnActor<DemoActor>();
	demoActor->GetRootComponent()->SetLocalPosition({ -300.0f, 0.0f, 1.0f });

	auto* demoActor2 = world.SpawnActor<DemoActor>();

	// auto demoUIActor = world->SpawnActor<DemoUIActor>();

	// auto demoAnimatorActor = world.SpawnActor<DemoAnimationActor>();
}