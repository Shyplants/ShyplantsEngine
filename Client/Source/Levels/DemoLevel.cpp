#include "ClientPCH/ClientPCH.h"
#include "Levels/DemoLevel.h"
#include "Actors/DemoActor.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/CameraComponent.h"

// Debug
#include "Engine/Graphics/D3D11/D3D11Renderer.h"

DemoLevel::DemoLevel(World* world)
	: Level(world)
{
}

DemoLevel::~DemoLevel()
{
}

void DemoLevel::OnBeginPlay()
{
	World* world = GetWorld();

	Actor* camActor = world->SpawnActor<Actor>();
	auto camera = camActor->AddComponent<CameraComponent>(camActor);
	camActor->GetRootComponent()->SetLocalPosition({ 0.0f, 100.0f, 0 });

	auto renderer = Engine::Get().GetRenderer();
	camera->SetOrthographic(renderer->INL_GetScreenWidth(), renderer->INL_GetScreenHeight(), 0.1f, 1000.0f);

	world->SetActiveCamera(camera);

	auto demoActor = world->SpawnActor<DemoActor>();
	demoActor->GetRootComponent()->SetLocalPosition({ 0,0,0.15f });
}

void DemoLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
}