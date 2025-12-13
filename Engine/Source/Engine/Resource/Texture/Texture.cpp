#include "Engine/Core/EnginePCH.h"
#include "Engine/Resource/Texture/Texture.h"
#include "Engine/Resource/Texture/TextureLoader.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"

Texture::Texture(D3D11Renderer* renderer)
	: Resource(ResourceType::Texture)
	, m_renderer(renderer)
{
}

Texture::~Texture()
{
	Unload();
}

bool Texture::Load(const std::wstring& path)
{
	// Texture는 기본적으로 SRGB로 로드
	return Load(path, TextureColorSpace::SRGB);
}

bool Texture::Load(const std::wstring& path, TextureColorSpace colorSpace)
{
	Unload();

	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	if (!TextureLoader::LoadFromFile(m_renderer->INL_GetDevice(), 
		m_renderer->INL_GetContext(), 
		path,
		colorSpace,
		resource, 
		m_srv))
	{
		return false;
	}

	// Resource -> Texture2D
	resource.As(&m_texture);

	if (m_texture)
	{
		D3D11_TEXTURE2D_DESC desc = {};
		m_texture->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
	}

	return true;
}

void Texture::Unload()
{
	m_srv.Reset();
	m_texture.Reset();
	m_width = 0;
	m_height = 0;
}
