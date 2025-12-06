#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Engine.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/System/InputManager.h"
#include "Engine/System/TimeManager.h"

#include "Engine/Core/World/World.h"

Engine* Engine::Instance = nullptr;

void Engine::Create()
{
	if (!Instance)
		Instance = new Engine();
}

void Engine::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

Engine& Engine::Get()
{
	assert(Instance && "Engine::Create() was not called");
	return *Instance;
}

Engine::Engine()
{

}

Engine::~Engine()
{
	m_world.reset();

	m_renderer.reset();
	ResourceManager::Destroy();
	InputManager::Destroy();
}

bool Engine::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	// Renderer 积己
	m_renderer = std::make_unique<D3D11Renderer>();
	if (!m_renderer->Init(hWnd))
	{
		__debugbreak();
		return false;
	}

	// ResourceManager 积己
	ResourceManager::Create();
	ResourceManager& resourceManager = ResourceManager::Get();
	if (!resourceManager.Init(m_renderer.get()))
	{
		__debugbreak();
		return false;
	}

	// InputManager 积己
	InputManager::Create();

	// TimeManager 积己
	TimeManager::Create();

	// World 积己
	m_world = std::make_unique<World>();

	return true;
}

void Engine::Run()
{
	// begin
	uint64 curTick = GetTickCount64();

	TickOnce(curTick);
	Render();

	if (m_fps != TimeManager::Get().GetFPS())
	{
		m_fps = TimeManager::Get().GetFPS();

		WCHAR wchText[64];
		DWORD dwTextLen = swprintf_s(wchText, L"FPS: %u", m_fps);

		SetWindowText(m_hWnd, wchText);
	}
}

bool Engine::TickOnce(uint64 curTick)
{
	auto& timeManager = TimeManager::Get();
	timeManager.Update(curTick);

	float deltaTime = timeManager.GetDeltaTime();

	// 60 FPS 力茄
	static float accumulator = 0.0f;
	accumulator += deltaTime;

	const float TARGET_DT = 1.0f / 60.0f;
	if (accumulator < TARGET_DT)
		return false;

	accumulator -= TARGET_DT;


	// 虐 涝仿 诀单捞飘
	InputManager::Get().Update();
	
	if (m_world)
		m_world->Tick(deltaTime);

	return true;
}

void Engine::Render()
{
	m_renderer->BeginRender();

	if (m_world)
		m_world->Render(*m_renderer);

	m_renderer->EndRender();
	m_renderer->Present();
}
