#pragma once

#include "Engine/Core/Component/ActorComponent.h"

class D3D11Renderer;
class SceneComponent;

class RendererComponent : public ActorComponent
{
public:
	explicit RendererComponent(Actor* owner);
	~RendererComponent() override;

	// Actor가 렌더링을 호출할 때만 실행됨
	virtual void Render(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj) = 0;

public:
	void SetVisible(bool visible) { m_visible = visible; }
	bool IsVisible() const { return m_visible; }

protected:
	SceneComponent* GetSceneComponent() const;

protected:
	bool m_visible{ true };
};