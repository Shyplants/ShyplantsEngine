#include "Engine/Core/EnginePCH.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include <d3dcompiler.h>

GraphicsPSO::GraphicsPSO()
{
}

GraphicsPSO::~GraphicsPSO()
{
	Cleanup();
}

bool GraphicsPSO::Init(D3D11Renderer* renderer, const std::wstring& fileName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutCount)
{
	m_renderer = renderer;

	m_blendState = renderer->GetDefaultBlendState();
	m_rasterizerState = renderer->GetDefaultRasterizerState();
	m_depthStencilState = renderer->GetDefaultDepthStencilState();

	const std::wstring shaderFolderPath = L"../Shaders/";
	const std::wstring vs_shaderFilePath = shaderFolderPath + L"VS_" + fileName + L".hlsl";
	const std::wstring ps_shaderFilePath = shaderFolderPath + L"PS_" + fileName + L".hlsl";

	if (!CreateVertexShader(vs_shaderFilePath) || !CreatePixelShader(ps_shaderFilePath) || !CreateInputLayout(layout, layoutCount))
	{
		__debugbreak();
		return false;
	}

	// VS遂 CB持失
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = 256;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (FAILED(m_renderer->INL_GetDevice()->CreateBuffer(&desc, nullptr, m_VSConstantBuffer.GetAddressOf())))
		{
			__debugbreak();
			return false;
		}
	}
	// Hash 持失
	m_hashID = 0;
	m_hashID = CombineHash(m_hashID, reinterpret_cast<uint64_t>(m_inputLayout.Get()));
	m_hashID = CombineHash(m_hashID, reinterpret_cast<uint64_t>(m_vertexShader.Get()));
	m_hashID = CombineHash(m_hashID, reinterpret_cast<uint64_t>(m_pixelShader.Get()));
	m_hashID = CombineHash(m_hashID, reinterpret_cast<uint64_t>(m_blendState));
	m_hashID = CombineHash(m_hashID, reinterpret_cast<uint64_t>(m_rasterizerState));
	m_hashID = CombineHash(m_hashID, reinterpret_cast<uint64_t>(m_depthStencilState));
	m_hashID = CombineHash(m_hashID, static_cast<uint64_t>(m_primitive));

	return true;
}

void GraphicsPSO::Cleanup()
{
	m_inputLayout.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
	m_vsBlob.Reset();
	m_psBlob.Reset();
}

bool GraphicsPSO::CreateVertexShader(const std::wstring& path)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* error = nullptr;
	HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, "VS_Main", "vs_5_0", flags, 0, m_vsBlob.GetAddressOf(), &error);

	if (FAILED(hr))
	{
		if (error)
		{
			OutputDebugStringA(static_cast<const char*>(error->GetBufferPointer()));
			error->Release();
		}
		return false;
	}

	auto device = m_renderer->INL_GetDevice();
	return SUCCEEDED(device->CreateVertexShader(
		m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()));
}

bool GraphicsPSO::CreatePixelShader(const std::wstring& path)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* error = nullptr;
	HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, "PS_Main", "ps_5_0", flags, 0, m_psBlob.GetAddressOf(), &error);

	if (FAILED(hr))
	{
		if (error)
		{
			OutputDebugStringA(static_cast<const char*>(error->GetBufferPointer()));
			error->Release();
		}
		return false;
	}

	auto device = m_renderer->INL_GetDevice();
	return SUCCEEDED(device->CreatePixelShader(
		m_psBlob->GetBufferPointer(),
		m_psBlob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf()));
}

bool GraphicsPSO::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* layout, UINT count)
{
	auto device = m_renderer->INL_GetDevice();
	return SUCCEEDED(device->CreateInputLayout(
		layout,
		count,
		m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(),
		m_inputLayout.GetAddressOf()));
}

uint64_t GraphicsPSO::CombineHash(uint64_t A, uint64_t B)
{
	A ^= B + 0x9e3779b97f4a7c15 + (A << 6) + (A >> 2);
	return A;
}
