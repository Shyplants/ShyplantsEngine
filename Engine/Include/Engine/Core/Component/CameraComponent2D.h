#pragma once

#include "Engine/Core/Component/SceneComponent.h"
#include "DirectXMath.h"
#include "Common/Types.h"

class D3D11Renderer;

class CameraComponent2D : public SceneComponent
{
public:
	CameraComponent2D(Actor* owner);
	~CameraComponent2D() override;

public:
	void SetZoom(float zoom);
	float GetZoom() const { return m_zoom; }

	void SetNearZ(float nearZ);
	void SetFarZ(float farZ);

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjMatrix() const;
	DirectX::XMMATRIX GetViewProjMatrix() const;

	DirectX::XMFLOAT2 ScreenToWorld(const DirectX::XMFLOAT2& screenPos, uint32 screenW, uint32 screenH) const;
	DirectX::XMFLOAT2 WorldToScreen(const DirectX::XMFLOAT2& worldPos, uint32 screenW, uint32 screenH) const;

	void SetAsMainCamera(bool b) { m_isMainCamera = b; }
	bool IsMainCamera() const { return m_isMainCamera; }

protected:
	void UpdateViewMatrix() const;
	void UpdateProjMatrix(uint32 viewportW, uint32 viewportH) const;
	void UpdateViewProjMatrix(uint32 viewportW, uint32 viewportH) const;

	void MarkViewDirty();
	void MarkProjDirty();

private:
	float m_zoom{ 1.0f };
	float m_nearZ{ 1.0f };
	float m_farZ{ 1000.0f };

	mutable bool m_viewDirty{ true };
	mutable bool m_projDirty{ true };
	mutable bool m_viewProjDirty{ true };

	mutable DirectX::XMMATRIX m_viewMatrix = DirectX::XMMatrixIdentity();
	mutable DirectX::XMMATRIX m_projMatrix = DirectX::XMMatrixIdentity();
	mutable DirectX::XMMATRIX m_viewProjMatrix = DirectX::XMMatrixIdentity();

	bool m_isMainCamera{ false };
};