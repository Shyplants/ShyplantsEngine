#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;

SceneComponent::SceneComponent(Actor* owner)
	: ActorComponent(owner)
{
}

SceneComponent::~SceneComponent()
{
	Detach();

	for (SceneComponent* child : m_children)
	{
		child->Detach(FDetachmentTransformRules::KeepWorldTransform);
	}

	m_children.clear();
}

void SceneComponent::SetLocalPosition(const DirectX::XMFLOAT3& pos)
{
	m_localPosition = pos;
	MarkDirtyRecursive();
}

void SceneComponent::SetLocalRotation(const DirectX::XMFLOAT3& rot)
{
	m_localRotation = rot;
	MarkDirtyRecursive();
}

void SceneComponent::SetLocalScale(const DirectX::XMFLOAT3& scale)
{
	m_localScale = scale;
	MarkDirtyRecursive();
}

XMFLOAT3 SceneComponent::GetWorldPosition()
{
	XMMATRIX W = GetWorldMatrix();
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, W.r[3]);

	return pos;
}

XMFLOAT3 SceneComponent::GetWorldRotation()
{
	XMMATRIX W = GetWorldMatrix();

	// Decompose
	XMVECTOR scaleV, rotQ, transV;
	XMMatrixDecompose(&scaleV, &rotQ, &transV, W);

	// Quaternion → Euler (radians)
	XMFLOAT3 eulerRad;
	XMStoreFloat3(&eulerRad, XMQuaternionRotationMatrix(W));

	// Convert to degrees
	return XMFLOAT3(
		XMConvertToDegrees(eulerRad.x),
		XMConvertToDegrees(eulerRad.y),
		XMConvertToDegrees(eulerRad.z)
	);
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

XMMATRIX SceneComponent::GetLocalMatrix() const
{
	XMMATRIX S = XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);
	XMMATRIX R = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_localRotation.x),
		XMConvertToRadians(m_localRotation.y),
		XMConvertToRadians(m_localRotation.z)
	);
	XMMATRIX T = XMMatrixTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);
	
	return S * R * T;
}

XMMATRIX SceneComponent::GetWorldMatrix() const
{
	if (!m_worldDirty)
		return m_cachedWorldMatrix;

	if (m_parent)
		m_cachedWorldMatrix = GetLocalMatrix() * m_parent->GetWorldMatrix();
	else
		m_cachedWorldMatrix = GetLocalMatrix();

	m_worldDirty = false;
	return m_cachedWorldMatrix;
}

void SceneComponent::AttachTo(SceneComponent* parent, const FAttachmentTransformRules& rules)
{
	if (!parent || parent == this)
		return;

	// Attach전 기존 월드 매트릭스 보관
	XMMATRIX oldWorld = GetWorldMatrix();

	// 기존 부모 제거
	if (m_parent)
		m_parent->RemoveChild(this);

	// 새로운 부모 지정
	m_parent = parent;
	parent->AddChild(this);

	// Transform rule 적용
	if (rules.bMaintainWorld)
	{
		XMMATRIX parentWorld = parent->GetWorldMatrix();
		XMMATRIX invParent = XMMatrixInverse(nullptr, parentWorld);

		// New local = World * Parent^-1
		XMMATRIX newLocal = oldWorld * invParent;

		XMVECTOR scaleV, rotQ, transV;
		XMMatrixDecompose(&scaleV, &rotQ, &transV, newLocal);

		XMFLOAT3 scale, pos;
		XMStoreFloat3(&scale, scaleV);
		XMStoreFloat3(&pos, transV);

		// Convert quaternion → euler degrees
		XMFLOAT3 eulerRad;
		XMStoreFloat3(&eulerRad, XMQuaternionRotationMatrix(newLocal));

		XMFLOAT3 eulerDeg(
			XMConvertToDegrees(eulerRad.x),
			XMConvertToDegrees(eulerRad.y),
			XMConvertToDegrees(eulerRad.z)
		);

		SetLocalScale(scale);
		SetLocalPosition(pos);
		SetLocalRotation(eulerDeg);
	}

	MarkDirtyRecursive();
}

void SceneComponent::Detach(const FDetachmentTransformRules& rules)
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

		XMFLOAT3 eulerRad;
		XMStoreFloat3(&eulerRad, XMQuaternionRotationMatrix(oldWorld));

		XMFLOAT3 eulerDeg(
			XMConvertToDegrees(eulerRad.x),
			XMConvertToDegrees(eulerRad.y),
			XMConvertToDegrees(eulerRad.z)
		);

		SetLocalScale(scale);
		SetLocalPosition(pos);
		SetLocalRotation(eulerDeg);
	}

	MarkDirtyRecursive();
}

void SceneComponent::AddChild(SceneComponent* child)
{
	if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
}

void SceneComponent::RemoveChild(SceneComponent* child)
{
	m_children.erase(
		std::remove(m_children.begin(), m_children.end(), child),
		m_children.end()
	);
}

void SceneComponent::MarkDirtyRecursive()
{
	m_worldDirty = true;

	for (SceneComponent* child : m_children)
		child->MarkDirtyRecursive();
}

