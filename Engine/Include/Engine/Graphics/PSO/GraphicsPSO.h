#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include <memory>
#include <cstdint>

class D3D11Renderer;

class GraphicsPSO
{
public:
	GraphicsPSO();
	~GraphicsPSO();

	GraphicsPSO(const GraphicsPSO&) = delete;
	GraphicsPSO& operator=(const GraphicsPSO&) = delete;
	GraphicsPSO(GraphicsPSO&&) = delete;
	GraphicsPSO& operator=(GraphicsPSO&&) = delete;

	bool operator==(const GraphicsPSO& rhs) const
	{
		return m_inputLayout.Get() == rhs.m_inputLayout.Get() &&
			m_vertexShader.Get() == rhs.m_vertexShader.Get() &&
			m_pixelShader.Get() == rhs.m_pixelShader.Get() &&
			m_primitive == rhs.m_primitive &&
			m_blendState == rhs.m_blendState &&
			m_depthStencilState == rhs.m_depthStencilState &&
			m_rasterizerState && rhs.m_rasterizerState;
	}

public:
	bool Init(D3D11Renderer* renderer, const std::wstring& fileName, const D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutCount);

	uint64_t GetHashID() const { return m_hashID; }

public:
	ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
	ID3D11VertexShader* GetVertexShader() const { return m_vertexShader.Get(); }
	ID3D11PixelShader* GetPixelShader() const { return m_pixelShader.Get(); }

	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return m_primitive; }

	ID3D11BlendState* GetBlendState() const { return m_blendState; }
	ID3D11RasterizerState* GetRasterizerState() const { return m_rasterizerState; }
	ID3D11DepthStencilState* GetDepthStencilState() const { return m_depthStencilState; }

	ID3D11Buffer* GetVSConstantBuffer() const { return m_VSConstantBuffer.Get(); }

private:
	void Cleanup();

	bool CreateVertexShader(const std::wstring& path);
	bool CreatePixelShader(const std::wstring& path);
	bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* layout, UINT count);

	uint64_t CombineHash(uint64_t A, uint64_t B);

private:
	D3D11Renderer* m_renderer{ nullptr };

	

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlob;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VSConstantBuffer{ nullptr };

	D3D11_PRIMITIVE_TOPOLOGY m_primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ID3D11BlendState* m_blendState{ nullptr };
	ID3D11RasterizerState* m_rasterizerState{ nullptr };
	ID3D11DepthStencilState* m_depthStencilState{ nullptr };

	uint64_t m_hashID{ 0 };
};