#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/TextRendererComponent.h"
#include "Engine/Core/Component/SceneComponent.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"

#include "SpriteFont.h"

using namespace DirectX;

TextRendererComponent::TextRendererComponent(Actor* owner)
	: RendererComponent(owner)
{
	m_font = GetRenderer().GetDefaultFont();
}

TextRendererComponent::~TextRendererComponent()
{
}

void TextRendererComponent::SetText(const std::wstring& text)
{
	m_text = text;
}

void TextRendererComponent::SetColor(const XMFLOAT4& color)
{
	m_color = color;
}

void TextRendererComponent::SetScale(float scale)
{
	m_scale = scale;
}

void TextRendererComponent::SetFont(SpriteFont* font)
{
	m_font = font;
}

void TextRendererComponent::RenderUI(D3D11Renderer& renderer)
{
	if (!m_font || m_text.empty())
		return;

	// 월드 좌표 -> 화면 좌표 변환을 위해 Actor의 SceneComponent 기준 transform 사용
	SceneComponent* sc = GetAttachComponent();
	XMFLOAT3 pos = sc->GetWorldPosition();

	m_font->DrawString(
		renderer.GetSpriteBatch(),
		m_text.c_str(),
		XMFLOAT2(pos.x, pos.y),
		XMLoadFloat4(&m_color),
		0.0f,
		XMFLOAT2(0, 0),
		m_scale
	);
}
