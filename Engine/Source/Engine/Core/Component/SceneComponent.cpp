#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;

SceneComponent::SceneComponent(Actor* owner)
	: ActorComponent(owner)
{
}

SceneComponent::~SceneComponent() = default;

// ======================================================
// Local Transform Setters
// ======================================================

void SceneComponent::SetLocalPosition(const XMFLOAT3& pos)
{
	m_localPosition = pos;
	MarkDirtyRecursive();
}

void SceneComponent::SetLocalRotation(const XMFLOAT3& rot)
{
	m_localRotation = rot;
	MarkDirtyRecursive();
}

void SceneComponent::SetLocalScale(const XMFLOAT3& scale)
{
	m_localScale = scale;
	MarkDirtyRecursive();
}

// ======================================================
// World Queries
// ======================================================

XMFLOAT3 SceneComponent::GetWorldPosition()
{
	XMVECTOR posV = XMVector3TransformCoord(
		XMVectorZero(),
		GetWorldMatrix());

	XMFLOAT3 pos;
	XMStoreFloat3(&pos, posV);

	return pos;
}

XMFLOAT3 SceneComponent::GetWorldPositionFast() const
{
	// Fast path: no parent → local == world
	if (m_parent == nullptr)
	{
		return m_localPosition;
	}

	// Ensure world matrix is up to date
	const XMMATRIX& world = GetWorldMatrix();

	// Extract translation directly from matrix
	XMFLOAT3 worldPos;
	worldPos.x = world.r[3].m128_f32[0];
	worldPos.y = world.r[3].m128_f32[1];
	worldPos.z = world.r[3].m128_f32[2];

	return worldPos;
}

XMFLOAT3 SceneComponent::GetWorldScale()
{
	XMMATRIX W = GetWorldMatrix();

	XMVECTOR scaleV, rotQ, transV;
	XMMatrixDecompose(&scaleV, &rotQ, &transV, W);

	XMFLOAT3 scale;
	XMStoreFloat3(&scale, scaleV);
	return scale;
}

// ======================================================
// Matrix Builders
// ======================================================

XMMATRIX SceneComponent::GetLocalMatrix() const
{
	XMMATRIX S = XMMatrixScaling(
		m_localScale.x,
		m_localScale.y,
		m_localScale.z);

	XMMATRIX R = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_localRotation.x),
		XMConvertToRadians(m_localRotation.y),
		XMConvertToRadians(m_localRotation.z));

	XMMATRIX T = XMMatrixTranslation(
		m_localPosition.x,
		m_localPosition.y,
		m_localPosition.z);

	return S * R * T;
}

XMMATRIX SceneComponent::GetWorldMatrix() const
{
	if (!m_worldDirty)
		return m_cachedWorldMatrix;

	if (m_parent)
	{
		m_cachedWorldMatrix =
			m_parent->GetWorldMatrix() * GetLocalMatrix();
	}
	else
	{
		m_cachedWorldMatrix = GetLocalMatrix();
	}

	m_worldDirty = false;
	return m_cachedWorldMatrix;
}

// ======================================================
// Attachment
// ======================================================

void SceneComponent::AttachTo(
	SceneComponent* parent,
	const FAttachmentTransformRules& rules)
{
	if (!parent || parent == this)
		return;

	XMMATRIX oldWorld = GetWorldMatrix();

	if (m_parent)
		m_parent->RemoveChild(this);

	m_parent = parent;
	parent->AddChild(this);

	if (rules.bMaintainWorld)
	{
		XMMATRIX parentWorld = parent->GetWorldMatrix();
		XMMATRIX invParent = XMMatrixInverse(nullptr, parentWorld);

		XMMATRIX newLocal = oldWorld * invParent;

		XMVECTOR scaleV, rotQ, transV;
		XMMatrixDecompose(&scaleV, &rotQ, &transV, newLocal);

		XMFLOAT3 scale, pos;
		XMStoreFloat3(&scale, scaleV);
		XMStoreFloat3(&pos, transV);

		SetLocalScale(scale);
		SetLocalPosition(pos);
		// 회전은 정책적으로 유지 (Euler 직접 복원은 위험)
	}

	MarkDirtyRecursive();
}

void SceneComponent::Detach(
	const FDetachmentTransformRules& rules)
{
	if (!m_parent)
		return;

	XMMATRIX oldWorld = GetWorldMatrix();

	m_parent->RemoveChild(this);
	m_parent = nullptr;

	if (rules.bMaintainWorld)
	{
		XMVECTOR scaleV, rotQ, transV;
		XMMatrixDecompose(&scaleV, &rotQ, &transV, oldWorld);

		XMFLOAT3 scale, pos;
		XMStoreFloat3(&scale, scaleV);
		XMStoreFloat3(&pos, transV);

		SetLocalScale(scale);
		SetLocalPosition(pos);
	}

	MarkDirtyRecursive();
}

// ======================================================
// Children
// ======================================================

void SceneComponent::AddChild(SceneComponent* child)
{
	if (std::find(m_children.begin(), m_children.end(), child)
		== m_children.end())
	{
		m_children.push_back(child);
	}
}

void SceneComponent::RemoveChild(SceneComponent* child)
{
	m_children.erase(
		std::remove(m_children.begin(), m_children.end(), child),
		m_children.end());
}

// ======================================================
// Dirty Propagation
// ======================================================

void SceneComponent::MarkDirtyRecursive()
{
	m_worldDirty = true;
	OnTransformDirty();

	for (SceneComponent* child : m_children)
	{
		child->MarkDirtyRecursive();
	}
}
