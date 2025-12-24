#include "ClientPCH/ClientPCH.h"

#include "Game/Levels/DemoLevel.h"
#include "Game/Levels/DemoLevelResources.h"

#include "Engine/Core/World/World.h"
// #include "Game/Actors/DemoUIActor.h"
// #include "Game/Actors/DemoAnimationActor.h"

DemoLevel::DemoLevel()
	: GameplayLevel()
{
	SetResourceSet(std::make_unique<DemoLevelResources>());
}

DemoLevel::~DemoLevel() = default;

void DemoLevel::OnEnter(World& world)
{
	GameplayLevel::OnEnter(world);
}

void DemoLevel::OnBeginPlay()
{
	GameplayLevel::OnBeginPlay();

	SP_LOG(LogGame, ELogLevel::Info, "DemoLevel::OnBeginPlay called");

	// auto demoUIActor = world->SpawnActor<DemoUIActor>();

	// auto demoAnimatorActor = world.SpawnActor<DemoAnimationActor>();
}