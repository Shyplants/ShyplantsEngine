#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"

#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/Component/CameraComponent2D.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/Mesh/SpriteMesh.h"
#include "Engine/Graphics/Command/DrawCommand.h"
#include "Engine/Resource/Texture/Texture.h"


using namespace DirectX;

SpriteRendererComponent::SpriteRendererComponent(Actor* owner)
	: RendererComponent(owner)
{
}

SpriteRendererComponent::~SpriteRendererComponent()
{
}

void SpriteRendererComponent::OnRegister()
{

}

void SpriteRendererComponent::RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj)
{
	if (!m_visible || !m_texture)
		return;

	if (!m_pso)
		m_pso = renderer.GetPSO(L"SpritePSO");

	if (!m_mesh)
		m_mesh = renderer.GetSpriteMesh();

	SceneComponent* sc = GetAttachComponent();
	if (!sc)
		return;

	float spriteW = static_cast<float>(m_rect.right - m_rect.left);
	float spriteH = static_cast<float>(m_rect.bottom - m_rect.top);

	// Pivot 계산
	XMFLOAT2 pivotPixel = { 0.0f, 0.0f };

	switch (m_pivotMode)
	{
	case SpritePivot::TopLeft:
		pivotPixel = { 0.0f, spriteH };
		break;

	case SpritePivot::TopCenter:
		pivotPixel = { spriteW * 0.5f, spriteH };
		break;

	case SpritePivot::TopRight:
		pivotPixel = { spriteW, spriteH };
		break;

	case SpritePivot::Center:
	case SpritePivot::MiddleLeft:
		pivotPixel = { 0.0f, spriteH * 0.5f };
		break;

	case SpritePivot::MiddleCenter:
		pivotPixel = { spriteW * 0.5f, spriteH * 0.5f };
		break;

	case SpritePivot::MiddleRight:
		pivotPixel = { spriteW, spriteH * 0.5f };
		break;

	case SpritePivot::BottomLeft:
		pivotPixel = { 0.0f, 0.0f };
		break;

	case SpritePivot::BottomCenter:
		pivotPixel = { spriteW * 0.5f, 0.0f };
		break;

	case SpritePivot::BottomRight:
		pivotPixel = { spriteW, 0.0f };
		break;

	default:
		break;
	}

	// Actor의 World Transform
	XMMATRIX W = sc->GetWorldMatrix();
	
	// Pixel Offset 적용
	XMMATRIX T_offset = XMMatrixTranslation(m_renderOffset.x, m_renderOffset.y, 0.0f);

	// Pivot 보정
	XMMATRIX T_pivot = XMMatrixTranslation(-pivotPixel.x, -pivotPixel.y, 0);

	// Scaling
	XMMATRIX S = XMMatrixScaling(spriteW * m_scale.x, spriteH * m_scale.y, 1.0f);


	// 최종 WVP 계산
	XMMATRIX world = S * T_pivot * T_offset * W;
	XMMATRIX wvp = world * viewProj;


	// DrawCommand 생성
	DrawCommand dc = {};

	// PSO/정렬 기준
	dc.PSOHashID = m_pso->GetHashID();
	dc.sortKey.PSOHashID = m_pso->GetHashID();
	dc.sortKey.renderLayer = m_gameLayer;
	dc.sortKey.contentLayer = m_orderInLayer;

	// 버퍼 정보
	dc.primitiveType = m_pso->GetPrimitiveType();
	dc.vertexBuffer = m_mesh->GetVertexBuffer();
	dc.vertexStride = m_mesh->GetVertexStride();
	dc.vertexOffset = m_mesh->GetVertexOffset();
	dc.indexBuffer = m_mesh->GetIndexBuffer();
	dc.indexFormat = DXGI_FORMAT_R16_UINT;
	dc.numIndices = m_mesh->GetIndexCount();

	// ConstantBuffer
	auto& cb = dc.spriteConstantBuffer;

	cb.WVP = XMMatrixTranspose(wvp);
	cb.Color = m_color;
	cb.Color.w *= m_alpha;

	// UV Rect 반영
	float texW = static_cast<float>(m_texture->GetWidth());
	float texH = static_cast<float>(m_texture->GetHeight());

	float invW = 1.0f / texW;
	float invH = 1.0f / texH;

	cb.TexCoord.x = m_rect.left * invW;
	cb.TexCoord.y = m_rect.top * invH;
	cb.TexSize.x = (m_rect.right - m_rect.left) * invW;
	cb.TexSize.y = (m_rect.bottom - m_rect.top) * invH;

	// Shader Resoruce
	dc.VSConstantBuffer = renderer.GetSpriteVSConstantBuffer();
	dc.PSTextureSRV = m_texture->GetSRV();
	dc.PSSampler = renderer.GetDefaultSamplerState();

	// Submit Command
	renderer.Submit(dc);
}

void SpriteRendererComponent::SetTexture(Texture* tex)
{
	m_texture = tex;
	
	if (m_texture)
		SetRectFullTexture();
}

void SpriteRendererComponent::SetRectFullTexture()
{
	if (m_texture)
	{
		m_rect.left = 0;
		m_rect.top = 0;
		m_rect.right = static_cast<LONG>(m_texture->GetWidth());
		m_rect.bottom = static_cast<LONG>(m_texture->GetHeight());
	}
	else
	{
		__debugbreak();
	}
}
