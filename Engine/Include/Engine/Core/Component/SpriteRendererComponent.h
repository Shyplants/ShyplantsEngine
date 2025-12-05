#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "DirectXMath.h"
#include "Engine/Graphics/Render/GameLayer.h"
#include "Engine/Graphics/Render/SpritePivot.h"

class Texture;
class SpriteMesh;
class GraphicsPSO;
class D3D11Renderer;

class SpriteRendererComponent : public RendererComponent
{
public:
	SpriteRendererComponent(Actor* owner);
	~SpriteRendererComponent() override;

	void OnRegister() override;
	void RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj) override;

public:

	// Sprite ¼³Á¤
	void SetTexture(Texture* tex);

	void SetRect(const RECT& rect) { m_rect = rect; }
	void SetRectFullTexture();

	void SetAlpha(float alpha) { m_alpha = alpha; }
	void SetDepth(float depth) { m_depth = depth; }
	void SetPivot(SpritePivot pivotMode) { m_pivotMode = pivotMode; }

	void SetScale(float sx, float sy) { m_scale = DirectX::XMFLOAT2(sx, sy); }
	void SetOffset(float ox, float oy) { m_offset = DirectX::XMFLOAT2(ox, oy); }
	
	void SetGameLayer(GameLayer layer) { m_gameLayer = layer; }
	void SetOrderInLayer(uint32 order) { m_orderInLayer = order; }

private:
	Texture* m_texture{ nullptr };
	SpriteMesh* m_mesh{ nullptr };
	GraphicsPSO* m_pso{ nullptr };

	DirectX::XMFLOAT2 m_scale{ 1.0f, 1.0f };
	DirectX::XMFLOAT2 m_offset{ 0.0f, 0.0f };
	DirectX::XMFLOAT4 m_color{ 1.0f,1.0f,1.0f,1.0f };
	RECT m_rect{ 0, 0, 0, 0 };

	SpritePivot m_pivotMode{ SpritePivot::Center };
	float m_depth{ 0.0f };
	float m_alpha{ 1.0f };

	GameLayer m_gameLayer{ 0 };
	uint32 m_orderInLayer{ 0 };
};