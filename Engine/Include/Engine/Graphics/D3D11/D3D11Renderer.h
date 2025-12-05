#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <functional>
#include <string>

#include "Common/Types.h"
#include "Engine/Graphics/RenderStates.h"

#include "SpriteBatch.h"
#include "SpriteFont.h"

const UINT SWAP_CHAIN_FRAME_COUNT = 2;

class GraphicsPSO;
class SpriteMesh;
class CommandBuffer;
struct DrawCommand;

struct SpriteConstantBuffer;

class D3D11Renderer
{
public:
	D3D11Renderer();
	~D3D11Renderer();

	D3D11Renderer(const D3D11Renderer&) = delete;
	D3D11Renderer& operator=(const D3D11Renderer&) = delete;
	D3D11Renderer(D3D11Renderer&&) = delete;
	D3D11Renderer& operator=(D3D11Renderer&&) = delete;

public:
	bool Init(HWND hWnd);

	void BeginRender();
	void EndRender();
	void Present();

	bool UpdateWindowSize(DWORD dwBackBufferWidth, DWORD dwBackBufferHeight);

public:
	CommandBuffer& GetCommandBuffer();
	void Submit(const DrawCommand& cmd);

public:
	// for internal
	ID3D11Device* INL_GetDevice() const { return m_device.Get(); }
	ID3D11DeviceContext* INL_GetContext() const { return m_immediateContext.Get(); }

	DWORD INL_GetScreenWidth() const { return m_dwWidth; }
	DWORD INL_GetScreenHeight() const { return m_dwHeight; }


	ID3D11BlendState* GetDefaultBlendState();
	ID3D11DepthStencilState* GetDefaultDepthStencilState();
	ID3D11RasterizerState* GetDefaultRasterizerState();
	ID3D11SamplerState* GetDefaultSamplerState();
	
	ID3D11Buffer* GetSpriteVSConstantBuffer();


public:
	// PSO
	void RegisterPSO(const std::wstring& key, std::unique_ptr<GraphicsPSO> pso);
	GraphicsPSO* GetPSO(const uint64 PSOHashID);
	GraphicsPSO* GetPSO(const std::wstring& key);

	SpriteMesh* GetSpriteMesh() const { return m_spriteMesh.get(); }

public:
	// Font
	DirectX::SpriteBatch* GetSpriteBatch() const { return m_spriteBatch.get(); }
	DirectX::SpriteFont* GetDefaultFont() const { return m_defaultFont.get(); }


private:
	bool CreateDepthStencil(UINT width, UINT height);

	void CreateBlendStates();
	void CreateRasterizerStates();
	void CreateDepthStencilStates();
	void CreateSamplerStates();

	void CreateConstantBuffers();

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendStates[static_cast<size_t>(BlendState::End)] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilStates[static_cast<size_t>(DepthStencilState::End)] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerStates[static_cast<size_t>(RasterizerState::End)] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerStates[static_cast<size_t>(SamplerState::End)] = {};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VSConstantBuffer{};

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_immediateContext{};

	Microsoft::WRL::ComPtr<IDXGISwapChain>       m_swapChain{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backBuffer{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView{};

	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer{};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView{};

	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D11_VIEWPORT m_viewPort = {};
	DWORD m_dwWidth = 0;
	DWORD m_dwHeight = 0;

	UINT m_dwSwapChainFlags = 0;

private:
	// PSO
	std::unordered_map<std::wstring, uint64> m_psoTable{};
	std::unordered_map<uint64, std::unique_ptr<GraphicsPSO>> m_psoCache{};
	std::mutex m_psoMutex;

private:
	std::unique_ptr<SpriteMesh> m_spriteMesh{ nullptr };
	std::unique_ptr<CommandBuffer> m_cmdBuffer{ nullptr };

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_defaultFont;
	
};