#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"

#include "Engine/Core/World/Actor.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Component/SceneComponent.h"

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

void SpriteRendererComponent::Render(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj)
{
	if (!m_visible)
		return;

	if (!m_pso)
		m_pso = renderer.GetPSO(L"SpritePSO");

	if (!m_mesh)
		m_mesh = renderer.GetSpriteMesh();

	SceneComponent* sc = GetSceneComponent();
	if (!sc)
		return;
	
	XMMATRIX S = XMMatrixScaling((float)m_texture->GetWidth(), 
		(float)m_texture->GetHeight(),
		1.0f);

	XMMATRIX wvp = S * sc->GetWorldMatrix() * viewProj;

	DrawCommand dc = {};

	// PSO/정렬 기준
	dc.PSOHashID = m_pso->GetHashID();
	dc.sortKey.PSOHashID = m_pso->GetHashID();
	dc.sortKey.renderLayer = 0;
	dc.sortKey.contentLayer = 0;

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

	float invW = 1.0f / m_texture->GetWidth();
	float invH = 1.0f / m_texture->GetHeight();

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
