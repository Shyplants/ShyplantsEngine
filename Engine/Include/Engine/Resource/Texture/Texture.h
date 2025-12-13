#pragma once

#include "Engine/Resource/Resource.h"
#include "Engine/Resource/Texture/TextureColorSpace.h"
#include "Common/Types.h"

#include <string>
#include <wrl/client.h>
#include <d3d11.h>

class D3D11Renderer;

class Texture : public Resource
{
public:
	explicit Texture(D3D11Renderer* renderer);
	~Texture() override;

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

public:
	bool Load(const std::wstring& path) override;
	bool Load(const std::wstring& path, TextureColorSpace colorSpace);
	void Unload() override;

public:
	ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }
	ID3D11Texture2D* GetTexture() const { return m_texture.Get(); }

	uint32 GetWidth() const { return m_width; }
	uint32 GetHeight() const { return m_height; }


private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;

	uint32 m_width{ 0 };
	uint32 m_height{ 0 };

	D3D11Renderer* m_renderer{ nullptr };
};