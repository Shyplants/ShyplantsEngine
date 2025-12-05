#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/CameraComponent2D.h"
#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Core/Engine.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;

CameraComponent2D::CameraComponent2D(Actor* owner)
	: SceneComponent(owner)
{
}

CameraComponent2D::~CameraComponent2D()
{
}

void CameraComponent2D::SetZoom(float zoom)
{
	m_zoom = std::max(0.01f, zoom);
	MarkViewDirty();
	MarkProjDirty();
}

void CameraComponent2D::SetNearZ(float nearZ)
{
	m_nearZ = nearZ;
	MarkProjDirty();
}

void CameraComponent2D::SetFarZ(float farZ)
{
	m_farZ = farZ;
	MarkProjDirty();
}

XMMATRIX CameraComponent2D::GetViewMatrix() const
{
	UpdateViewMatrix();
	return m_viewMatrix;
}

XMMATRIX CameraComponent2D::GetProjMatrix() const
{
	auto renderer = Engine::Get().GetRenderer();
	auto w = renderer->INL_GetScreenWidth();
	auto h = renderer->INL_GetScreenHeight();

	UpdateProjMatrix(w, h);
	return m_projMatrix;
}

XMMATRIX CameraComponent2D::GetViewProjMatrix() const
{
	auto renderer = Engine::Get().GetRenderer();
	auto w = renderer->INL_GetScreenWidth();
	auto h = renderer->INL_GetScreenHeight();

	UpdateViewProjMatrix(w, h);
	return m_viewProjMatrix;
}

XMFLOAT2 CameraComponent2D::ScreenToWorld(const DirectX::XMFLOAT2& screenPos, uint32 screenW, uint32 screenH) const
{
	XMMATRIX invVP = XMMatrixInverse(nullptr, GetViewProjMatrix());

	float x = (screenPos.x / screenW) * 2.0f - 1.0f;
	float y = 1.0f - (screenPos.y / screenH) * 2.0f;

	XMVECTOR posNDC = XMVectorSet(x, y, 0, 1);
	XMVECTOR worldV = XMVector3TransformCoord(posNDC, invVP);

	XMFLOAT2 out;
	XMStoreFloat2(&out, worldV);

	return out;
}

XMFLOAT2 CameraComponent2D::WorldToScreen(const DirectX::XMFLOAT2& worldPos, uint32 screenW, uint32 screenH) const
{
	XMVECTOR worldV = XMVectorSet(worldPos.x, worldPos.y, 0, 1);
	XMVECTOR clip = XMVector3TransformCoord(worldV, GetViewProjMatrix());

	XMFLOAT2 out;
	out.x = (XMVectorGetX(clip) * 0.5f + 0.5f) * screenW;
	out.y = (1.0f - (XMVectorGetY(clip) * 0.5f + 0.5f)) * screenH;

	return out;
}

void CameraComponent2D::UpdateViewMatrix() const
{
	if (!m_viewDirty)
		return;

	XMMATRIX world = GetWorldMatrix();

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, world.r[3]);

	m_viewMatrix = XMMatrixTranslation(-pos.x, -pos.y, 0);
	m_viewDirty = false;
}

void CameraComponent2D::UpdateProjMatrix(uint32 viewportW, uint32 viewportH) const
{
	if (!m_projDirty)
		return;

	float halfW = (viewportW * 0.5f) / m_zoom;
	float halfH = (viewportH * 0.5f) / m_zoom;

	m_projMatrix = XMMatrixOrthographicOffCenterLH(
		-halfW, +halfW,
		+halfH, -halfH,
		m_nearZ, m_farZ
	);

	m_projDirty = false;
}

void CameraComponent2D::UpdateViewProjMatrix(uint32 viewportW, uint32 viewportH) const
{
	if (!m_viewProjDirty)
		return;

	UpdateViewMatrix();
	UpdateProjMatrix(viewportW, viewportH);

	m_viewProjMatrix = m_viewMatrix * m_projMatrix;
	m_viewProjDirty = false;
}

void CameraComponent2D::MarkViewDirty()
{
	m_viewDirty = true;
	m_viewProjDirty = true;
}

void CameraComponent2D::MarkProjDirty()
{
	m_projDirty = true;
	m_viewProjDirty = true;
}