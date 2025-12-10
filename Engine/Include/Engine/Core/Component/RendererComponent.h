#pragma once

#include "Engine/Core/Component/ActorComponent.h"

class D3D11Renderer;
class SceneComponent;

class RendererComponent : public ActorComponent
{
public:
	explicit RendererComponent(Actor* owner);
	~RendererComponent() override;

	virtual void RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj) {}
	virtual void RenderUI(D3D11Renderer& renderer) {}
	virtual bool IsUIRenderer() const { return false; }

public:
	void SetVisible(bool visible) { m_visible = visible; }
	bool IsVisible() const { return m_visible; }

	void SetRenderOffset(const DirectX::XMFLOAT2& offset) { m_renderOffset = offset; }
	DirectX::XMFLOAT2 GetRenderOffset() const { return m_renderOffset; }

protected:
	SceneComponent* GetAttachComponent() const { return m_attachComponent; }

	// Renderer 접근 헬퍼
	D3D11Renderer& GetRenderer() const;

private:
	friend class Actor;

	// RenderComponent가 어느 SceneComponent에 붙어 있는지 설정
	void SetAttachComponent(SceneComponent* comp) { m_attachComponent = comp; }

protected:
	SceneComponent* m_attachComponent{ nullptr };

protected:
	bool m_visible{ true };
	
	DirectX::XMFLOAT2 m_renderOffset{ 0.0f, 0.0f };
};