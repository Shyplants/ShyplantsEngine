#include "Engine/Core/EnginePCH.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"

#include "Engine/Graphics/ConstantBuffer/SpriteConstantBuffer.h"
#include "Engine/Graphics/Vertex/SpriteVertex.h"
#include "Engine/Graphics/Mesh/SpriteMesh.h"

#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/Command/DrawCommand.h"
#include "Engine/Graphics/Command/CommandBuffer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

D3D11Renderer::D3D11Renderer()
{

}

D3D11Renderer::~D3D11Renderer()
{
}

bool D3D11Renderer::Init(HWND hWnd)
{
	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	D3D_FEATURE_LEVEL featureLevel;

	if (FAILED(D3D11CreateDevice(
		nullptr,
		driverType,
		0,
		createDeviceFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_device.GetAddressOf(),
		&featureLevel,
		m_immediateContext.GetAddressOf())))
	{
		return false;
	}

	RECT rect;
	::GetClientRect(hWnd, &rect);
	DWORD dwWndWidth = rect.right - rect.left;
	DWORD dwWndHeight = rect.bottom - rect.top;
	UINT dwBackBufferWidth = rect.right - rect.left;
	UINT dwBackBufferHeight = rect.bottom - rect.top;

	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = dwBackBufferWidth;
		swapChainDesc.Height = dwBackBufferHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		// swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = SWAP_CHAIN_FRAME_COUNT;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		m_dwSwapChainFlags = swapChainDesc.Flags;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		ComPtr<IDXGIDevice> pDevice;
		m_device.As(&pDevice);

		ComPtr<IDXGIAdapter> pAdapter;
		pDevice->GetAdapter(&pAdapter);

		ComPtr<IDXGIFactory2> pFactory;
		pAdapter->GetParent(IID_PPV_ARGS(&pFactory));

		ComPtr<IDXGISwapChain1> pSwapChain1;
		if (FAILED(pFactory->CreateSwapChainForHwnd(m_device.Get(), hWnd, &swapChainDesc, &fsSwapChainDesc, nullptr, &pSwapChain1)))
		{
			__debugbreak();
			return false;
		}

		pSwapChain1.As(&m_swapChain);
	}

	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer));
	m_device->CreateRenderTargetView(m_backBuffer.Get(), nullptr, &m_renderTargetView);

	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_viewPort.Width = static_cast<float>(dwWndWidth);
	m_viewPort.Height = static_cast<float>(dwWndHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_dwWidth = dwWndWidth;
	m_dwHeight = dwWndHeight;

	CreateDepthStencil(m_dwWidth, m_dwHeight);

	CreateBlendStates();
	CreateDepthStencilStates();
	CreateRasterizerStates();
	CreateSamplerStates();

	CreateConstantBuffers();

	m_spriteMesh = std::make_unique<SpriteMesh>();
	if (!m_spriteMesh->Init(m_device.Get()))
	{
		__debugbreak();
		return false;
	}

	auto spritePSO = std::make_unique<GraphicsPSO>();
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	if (!spritePSO->Init(this, L"Sprite", inputElementDescs, 3))
	{
		__debugbreak();
		return false;
	}

	RegisterPSO(L"SpritePSO", std::move(spritePSO));


	m_cmdBuffer = std::make_unique<CommandBuffer>();


	// Init Font
	m_spriteBatch = std::make_unique<SpriteBatch>(m_immediateContext.Get());
	m_defaultFont = std::make_unique<SpriteFont>(m_device.Get(), L"../Resources/Fonts/pretendard.spritefont");

	/*InitCamera();

	m_pFontManager = std::make_unique<CFontManager>();
	m_pFontManager->Init(m_device.Get(), m_pImmediateContext.Get(), 1024, 256, 96.0f, true);*/

	// CreateFontObject(L"Segoe UI", 24.0f);

	return true;

}

void D3D11Renderer::BeginRender()
{
	m_immediateContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	const float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_immediateContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_immediateContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_immediateContext->RSSetViewports(1, &m_viewPort);
}

void D3D11Renderer::EndRender()
{
	auto& cmds = m_cmdBuffer->AccessCommands();
	if (cmds.empty())
		return;

	std::sort(cmds.begin(), cmds.end(),
		[](const DrawCommand& A, const DrawCommand& B)
		{
			return A.sortKey < B.sortKey;
		});

	uint64 lastPSOHashID = 0;
	GraphicsPSO* currentPSO = nullptr;



	for (auto& cmd : cmds)
	{
		if (!(cmd.PSOHashID == lastPSOHashID))
		{
			currentPSO = GetPSO(cmd.PSOHashID);
			lastPSOHashID = cmd.PSOHashID;
		}

		cmd.Execute(m_immediateContext.Get(), currentPSO);
	}
	
	m_cmdBuffer->Clear();
}

void D3D11Renderer::Present()
{
	UINT presentFlags = 0;

	if (m_dwSwapChainFlags & DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING)
	{
		presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
		m_swapChain->Present(0, presentFlags);
	}
	else
	{
		m_swapChain->Present(1, 0);
	}
}

bool D3D11Renderer::UpdateWindowSize(DWORD dwBackBufferWidth, DWORD dwBackBufferHeight)
{
	if (!m_immediateContext || !m_swapChain)
	{
		return false;
	}

	if (dwBackBufferWidth * dwBackBufferHeight == 0)
	{
		return false;
	}

	if (m_dwWidth == dwBackBufferWidth && m_dwHeight == dwBackBufferHeight)
	{
		return false;
	}

	m_renderTargetView.Reset();
	m_backBuffer.Reset();
	m_depthStencilView.Reset();
	m_depthStencilBuffer.Reset();

	if (FAILED(m_swapChain->ResizeBuffers(SWAP_CHAIN_FRAME_COUNT, dwBackBufferWidth, dwBackBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM, m_dwSwapChainFlags)))
	{
		return false;
	}

	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer));
	m_device->CreateRenderTargetView(m_backBuffer.Get(), nullptr, &m_renderTargetView);

	m_viewPort.Width = static_cast<float>(dwBackBufferWidth);
	m_viewPort.Height = static_cast<float>(dwBackBufferHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_dwWidth = dwBackBufferWidth;
	m_dwHeight = dwBackBufferHeight;

	CreateDepthStencil(m_dwWidth, m_dwHeight);

	return true;
}

CommandBuffer& D3D11Renderer::GetCommandBuffer()
{
	return *m_cmdBuffer;
}

void D3D11Renderer::Submit(const DrawCommand& cmd)
{
	m_cmdBuffer->Submit(cmd);
}

ID3D11BlendState* D3D11Renderer::GetDefaultBlendState()
{
	return m_blendStates[static_cast<size_t>(BlendState::AlphaBlend)].Get();
}

ID3D11DepthStencilState* D3D11Renderer::GetDefaultDepthStencilState()
{
	return m_depthStencilStates[static_cast<size_t>(DepthStencilState::LessEqual)].Get();
}

ID3D11RasterizerState* D3D11Renderer::GetDefaultRasterizerState()
{
	return m_rasterizerStates[static_cast<size_t>(RasterizerState::SolidBack)].Get();
}

ID3D11SamplerState* D3D11Renderer::GetDefaultSamplerState()
{
	return m_samplerStates[static_cast<size_t>(SamplerState::Point)].Get();
}

ID3D11Buffer* D3D11Renderer::GetSpriteVSConstantBuffer()
{
	return m_VSConstantBuffer.Get();
}

void D3D11Renderer::RegisterPSO(const std::wstring& key, std::unique_ptr<GraphicsPSO> pso)
{
	std::scoped_lock lock(m_psoMutex);

	if (m_psoTable.find(key) != m_psoTable.end())
	{
		return;
	}

	auto hashID = pso->GetHashID();
	m_psoTable.emplace(key, hashID);
	m_psoCache.emplace(hashID, std::move(pso));
}

GraphicsPSO* D3D11Renderer::GetPSO(const uint64 PSOHashID)
{
	std::scoped_lock lock(m_psoMutex);
	
	auto it = m_psoCache.find(PSOHashID);
	return (it != m_psoCache.end()) ? it->second.get() : nullptr;
}

GraphicsPSO* D3D11Renderer::GetPSO(const std::wstring& key)
{
	std::scoped_lock lock(m_psoMutex);

	if (m_psoTable.find(key) == m_psoTable.end())
		return nullptr;
	
	auto hashID = m_psoTable[key];
	return m_psoCache[hashID].get();
}

bool D3D11Renderer::CreateDepthStencil(UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	m_device->CreateTexture2D(&depthStencilBufferDesc, nullptr, m_depthStencilBuffer.GetAddressOf());
	m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf());

	return true;
}

void D3D11Renderer::CreateBlendStates()
{
	D3D11_BLEND_DESC blendDesc;

	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_device->CreateBlendState(&blendDesc, m_blendStates[static_cast<size_t>(BlendState::AlphaBlend)].GetAddressOf());

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_device->CreateBlendState(&blendDesc, m_blendStates[static_cast<size_t>(BlendState::OneOne)].GetAddressOf());
}

void D3D11Renderer::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;

	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerStates[static_cast<size_t>(RasterizerState::SolidBack)].GetAddressOf());

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerStates[static_cast<size_t>(RasterizerState::SolidFront)].GetAddressOf());

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerStates[static_cast<size_t>(RasterizerState::SolidNone)].GetAddressOf());

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerStates[static_cast<size_t>(RasterizerState::Wireframe)].GetAddressOf());
}

void D3D11Renderer::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = false;
	m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilStates[static_cast<size_t>(DepthStencilState::LessEqual)].GetAddressOf());

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilStates[static_cast<size_t>(DepthStencilState::DepthNone)].GetAddressOf());
}

void D3D11Renderer::CreateSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_device->CreateSamplerState(&samplerDesc, m_samplerStates[static_cast<size_t>(SamplerState::Point)].GetAddressOf());

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_device->CreateSamplerState(&samplerDesc, m_samplerStates[static_cast<size_t>(SamplerState::Linear)].GetAddressOf());
}

void D3D11Renderer::CreateConstantBuffers()
{
	D3D11_BUFFER_DESC cbDesc = {};

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = sizeof(SpriteConstantBuffer);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	SpriteConstantBuffer cbSprite;
	D3D11_SUBRESOURCE_DATA cbData = {};
	cbData.pSysMem = &cbSprite;

	m_device->CreateBuffer(&cbDesc, &cbData, m_VSConstantBuffer.GetAddressOf());
}
