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
}

void SceneComponent::SetLocalPosition(const DirectX::XMFLOAT3& pos)
{
	m_localPosition = pos;
	MarkDirty();
}

void SceneComponent::SetLocalRotationQuat(const DirectX::XMFLOAT4& rot)
{
	m_localRotation = rot;
	MarkDirty();
}

void SceneComponent::SetLocalScale(const DirectX::XMFLOAT3& scale)
{
	m_localScale = scale;
	MarkDirty();
}

XMFLOAT3 SceneComponent::GetWorldPosition() const
{
	UpdateWorldTransform();
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, m_cachedWorldMatrix.r[3]);

	return pos;
}

DirectX::XMFLOAT4 SceneComponent::GetWorldRotationQuat() const
{
	UpdateWorldTransform();
	XMFLOAT4 quat;
	XMStoreFloat4(&quat, XMQuaternionRotationMatrix(m_cachedWorldMatrix));

	return quat;
}

XMFLOAT3 SceneComponent::GetWorldScale() const
{
	UpdateWorldTransform();
	XMFLOAT3 scale;
	scale.x = XMVectorGetX(XMVector3Length(m_cachedWorldMatrix.r[0]));
	scale.y = XMVectorGetX(XMVector3Length(m_cachedWorldMatrix.r[1]));
	scale.z = XMVectorGetX(XMVector3Length(m_cachedWorldMatrix.r[2]));

	return scale;
}

XMMATRIX SceneComponent::GetLocalMatrix() const
{
	XMMATRIX S = XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);
	XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&m_localRotation));
	XMMATRIX T = XMMatrixTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);
	
	return S * R * T;
}

XMMATRIX SceneComponent::GetWorldMatrix() const
{
	UpdateWorldTransform();
	return m_cachedWorldMatrix;
}

void SceneComponent::MarkDirty()
{
	m_dirty = true;

	for (SceneComponent* child : m_children)
		child->MarkDirty();
}

void SceneComponent::MarkClean()
{
	UpdateWorldTransform();
}

void SceneComponent::AttachTo(SceneComponent* parent)
{
	if (m_parent == parent)
		return;

	Detach();

	m_parent = parent;

	if (parent)
		parent->m_children.push_back(this);

	MarkDirty();
}

void SceneComponent::Detach()
{
	if (!m_parent)
		return;

	auto& siblings = m_parent->m_children;
	siblings.erase(
		std::remove(siblings.begin(), siblings.end(), this),
		siblings.end()
	);

	m_parent = nullptr;
	MarkDirty();
}

void SceneComponent::UpdateWorldTransform() const
{
	if (!m_dirty)
		return;

	XMMATRIX local = GetLocalMatrix();

	if (m_parent)
		m_cachedWorldMatrix = m_parent->GetWorldMatrix() * local;
	else
		m_cachedWorldMatrix = local;

	m_dirty = false;
}

