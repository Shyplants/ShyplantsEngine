#include "ClientPCH/ClientPCH.h"
#include "Levels/DemoLevel.h"
#include "Actors/DemoActor.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/World/World.h"
// #include "Engine/Core/Component/CameraComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"

// Debug
#include "Engine/Graphics/D3D11/D3D11Renderer.h"

DemoLevel::DemoLevel(World* world)
	: Level(world)
{
}

DemoLevel::~DemoLevel()
{
}

void DemoLevel::OnLoad()
{
	Actor* camActor = GetWorld()->SpawnActor<Actor>();
	auto cam = camActor->AddComponent<CameraComponent2D>(camActor);

	camActor->GetRootComponent()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	cam->SetZoom(1.0f);

	GetWorld()->SetMainCamera(cam);
}

void DemoLevel::OnBeginPlay()
{
	World* world = GetWorld();


	auto demoActor = world->SpawnActor<DemoActor>();
	demoActor->GetRootComponent()->SetLocalPosition({ 100.0f,0,0.15f });
}

void DemoLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
}