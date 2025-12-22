#include "ClientPCH/ClientPCH.h"
#include "Game/Levels/DemoLevel.h"

#include "Game/Actors/DemoActor.h"
// #include "Game/Actors/DemoUIActor.h"
// #include "Game/Actors/DemoAnimationActor.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/World/World.h"
// #include "Engine/Core/Component/CameraComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"


DemoLevel::DemoLevel() = default;

DemoLevel::~DemoLevel() = default;

void DemoLevel::OnEnter(World& world)
{
	Level::OnEnter(world);

	Actor* camActor = world.SpawnActor<Actor>();
	auto cam = camActor->AddComponent<CameraComponent2D>();

	camActor->GetRootComponent()->SetLocalPosition({ 0.0, 0.0f, -10.0f });
	
	cam->SetZoom(1.0f);
	world.SetActiveCamera(cam);
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