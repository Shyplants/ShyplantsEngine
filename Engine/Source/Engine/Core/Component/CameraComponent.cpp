#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/CameraComponent.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;

CameraComponent::CameraComponent(Actor* owner)
	: SceneComponent(owner)
	, m_projectionMode(CameraProjectionMode::Orthographic)
	, m_fovY(XMConvertToRadians(60.0f))
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Tick(float deltaTime)
{
	SceneComponent::Tick(deltaTime);

	if (IsDirty())
	{
		m_viewDirty = true;
		m_viewProjDirty = true;
		MarkClean();
	}
}

void CameraComponent::SetProjectionMode(CameraProjectionMode mode)
{
	if (m_projectionMode != mode)
	{
		m_projectionMode = mode;
		m_projDirty = true;
		m_viewProjDirty = true;
	}
}

void CameraComponent::SetPerspective(float fovYDeg, float aspect, float nearZ, float farZ)
{
	m_projectionMode = CameraProjectionMode::Perspective;

	m_fovY = XMConvertToRadians(fovYDeg);
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_projDirty = true;
	m_viewProjDirty = true;
}

void CameraComponent::SetOrthographic(float width, float height, float nearZ, float farZ)
{
	m_projectionMode = CameraProjectionMode::Orthographic;

	m_orthoWidth = width;
	m_orthoHeight = height;
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_projDirty = true;
	m_viewProjDirty = true;
}

XMMATRIX CameraComponent::GetViewMatrix() const
{
	UpdateViewMatrix();
	return m_viewMatrix;
}

XMMATRIX CameraComponent::GetProjMatrix() const
{
	UpdateProjMatrix();
	return m_projMatrix;
}

XMMATRIX CameraComponent::GetViewProjMatrix() const
{
	UpdateViewProjMatrix();
	return m_viewProjMatrix;
}

void CameraComponent::UpdateViewMatrix() const
{
	if (!m_viewDirty)
		return;

	XMMATRIX world = GetWorldMatrix();
	XMMATRIX inv = XMMatrixInverse(nullptr, world);
	m_viewMatrix = inv;

	m_viewDirty = false;
	m_viewProjDirty = true;
}

void CameraComponent::UpdateProjMatrix() const
{
	if (!m_projDirty)
		return;

	if (m_projectionMode == CameraProjectionMode::Perspective)
	{
		m_projMatrix =
			XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
	}
	else // Orthographic
	{
		float halfW = m_orthoWidth / 2.0f;
		float halfH = m_orthoHeight / 2.0f;

		/*m_projMatrix = XMMatrixOrthographicLH(m_orthoWidth, m_orthoHeight, m_nearZ, m_farZ);*/

		m_projMatrix =
			XMMatrixOrthographicOffCenterLH(
				-halfW, +halfW,
				-halfH, +halfH,    
				m_nearZ, m_farZ);
	}

	m_projDirty = false;
	m_viewProjDirty = true;
}

void CameraComponent::UpdateViewProjMatrix() const
{
	if (!m_viewProjDirty)
		return;

	UpdateViewMatrix();
	UpdateProjMatrix();

	m_viewProjMatrix = m_viewMatrix * m_projMatrix;

	m_viewProjDirty = false;
}
