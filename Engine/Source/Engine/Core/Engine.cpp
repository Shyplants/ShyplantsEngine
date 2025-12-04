#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Engine.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Resource/ResourceManager.h"
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
}

bool Engine::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	// Renderer 持失
	m_renderer = std::make_unique<D3D11Renderer>();
	if (!m_renderer->Init(hWnd))
	{
		__debugbreak();
		return false;
	}

	// ResourceManager 持失
	ResourceManager::Create();
	ResourceManager& resourceManager = ResourceManager::Get();
	if (!resourceManager.Init(m_renderer.get()))
	{
		__debugbreak();
		return false;
	}

	// World 持失
	m_world = std::make_unique<World>();

	return true;
}

void Engine::Run()
{
	m_frameCount++;

	// begin
	uint64 curTick = GetTickCount64();

	TickOnce(curTick);
	Render();

	if (curTick - m_prevFrameCheckTick > 1000)
	{
		m_prevFrameCheckTick = curTick;

		WCHAR wchText[64];
		DWORD dwTextLen = swprintf_s(wchText, L"FPS: %u", m_FPS);

		SetWindowText(m_hWnd, wchText);

		m_FPS = m_frameCount;
		m_frameCount = 0;
	}


}

bool Engine::TickOnce(uint64 curTick)
{
	uint64 deltaMs = curTick - m_prevUpdateTick;

	// 16ms (60fps)
	const uint64 TARGET_FRAME_MS = 1000 / 60;

	if (deltaMs < TARGET_FRAME_MS)
	{
		return false;
	}
	
	m_prevUpdateTick = curTick;

	float deltaTime = static_cast<float>(deltaMs) / 1000.0f;

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
