#pragma once

#include "Engine/Core/Component/SceneComponent.h"
#include "DirectXMath.h"

enum class CameraProjectionMode
{
	Perspective,
	Orthographic
};

class CameraComponent : public SceneComponent
{
public:
	CameraComponent(Actor* owner);
	~CameraComponent() override;

	void Tick(float deltaTime) override;

public:
	void SetProjectionMode(CameraProjectionMode mode);
	void SetPerspective(float fovYDeg, float aspect, float nearZ, float farZ);
	void SetOrthographic(float width, float height, float nearZ, float farZ);

	CameraProjectionMode GetProjectionMode() const { return m_projectionMode; }
	
	float GetFovY() const { return m_fovY; }
	float GetAspect() const { return m_aspect; }
	float GetNearZ() const { return m_nearZ; }
	float GetFarZ() const { return m_farZ; }

	float GetOrthoWidth() const { return m_orthoWidth; }
	float GetOrthoHeight() const { return m_orthoHeight; }

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjMatrix() const;
	DirectX::XMMATRIX GetViewProjMatrix() const;

private:
	void UpdateViewMatrix() const;
	void UpdateProjMatrix() const;
	void UpdateViewProjMatrix() const;

private:
	CameraProjectionMode m_projectionMode;

	float m_fovY{ 0.0f };
	float m_aspect{ 1.0f };
	float m_nearZ{ 0.1f };
	float m_farZ{ 1000.0f };

	// Orthographic params
	float m_orthoWidth{ 1280.0f };
	float m_orthoHeight{ 720.0f };

	// Matrices (cached)
	mutable bool m_viewDirty{ true };
	mutable bool m_projDirty{ true };
	mutable bool m_viewProjDirty{ true };

	mutable DirectX::XMMATRIX m_viewMatrix{};
	mutable DirectX::XMMATRIX m_projMatrix{};
	mutable DirectX::XMMATRIX m_viewProjMatrix{};
};