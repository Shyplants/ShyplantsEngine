#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "DirectXMath.h"
#include <string>

namespace DirectX
{
	inline namespace DX11
	{
		class SpriteFont;
	}
}

class D3D11Renderer;

class TextRendererComponent : public RendererComponent
{
public:
	TextRendererComponent(Actor* onwer);
	~TextRendererComponent() override;

	bool IsUIRenderer() const override { return true; }

public:
	void SetText(const std::wstring& text);
	void SetColor(const DirectX::XMFLOAT4& color);
	void SetScale(float scale);
	void SetFont(DirectX::SpriteFont* font);

protected:
	void RenderUI(D3D11Renderer& renderer) override;

private:
	std::wstring m_text = L"";
	DirectX::XMFLOAT4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_scale = 1.0f;

	DirectX::SpriteFont* m_font = nullptr;
};