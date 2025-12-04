#pragma once

#include "Common/Types.h"
#include <memory>

class D3D11Renderer;
class World;

class Engine
{
public:
	static void Create();
	static void Destroy();

	static Engine& Get();

private:
	Engine();
	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

public:
	bool Init(HWND hWnd);
	void Run();
	bool TickOnce(uint64 curTick);
	void Render();

public:
	D3D11Renderer* GetRenderer() const { return m_renderer.get(); }
	World* GetWorld() const { return m_world.get(); }

private:
	static Engine* Instance;

private:
	HWND m_hWnd{ nullptr };

	uint64 m_prevFrameCheckTick{ 0 };
	uint64 m_prevUpdateTick{ 0 };
	uint32 m_frameCount{ 0 };
	uint32 m_FPS{ 0 };

private:
	std::unique_ptr<D3D11Renderer> m_renderer{ nullptr };

	std::unique_ptr<World> m_world{ nullptr };

};