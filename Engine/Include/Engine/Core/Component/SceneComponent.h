#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "DirectXMath.h"
#include <vector>

class Actor;

class SceneComponent : public ActorComponent
{
public:
	SceneComponent(Actor* owner);
	~SceneComponent() override;

public:
	// Transform ¼³Á¤
	void SetLocalPosition(const DirectX::XMFLOAT3& pos);
	void SetLocalRotationQuat(const DirectX::XMFLOAT4& rot);
	void SetLocalScale(const DirectX::XMFLOAT3& scale);

	const DirectX::XMFLOAT3& GetLocalPosition() const { return m_localPosition; }
	const DirectX::XMFLOAT4& GetLocalRotationQuat() const { return m_localRotation; }
	const DirectX::XMFLOAT3& GetLocalScale() const { return m_localScale; }

	DirectX::XMFLOAT3 GetWorldPosition() const;
	DirectX::XMFLOAT4 GetWorldRotationQuat() const;
	DirectX::XMFLOAT3 GetWorldScale() const;
	

	DirectX::XMMATRIX GetLocalMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	// Dirty API
	bool IsDirty() const { return m_dirty; }
	void MarkDirty();
	void MarkClean();

	// Hierarchy
	void AttachTo(SceneComponent* parent);
	void Detach();

	SceneComponent* GetParent() const { return m_parent; }
	const std::vector<SceneComponent*>& GetChildren() const { return m_children; }

protected:
	void UpdateWorldTransform() const;

protected:
	DirectX::XMFLOAT3 m_localPosition{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 m_localRotation{ 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT3 m_localScale{ 1.0f, 1.0f, 1.0f };
	
	SceneComponent* m_parent{ nullptr };
	std::vector<SceneComponent*> m_children;

	mutable DirectX::XMMATRIX m_cachedWorldMatrix = DirectX::XMMatrixIdentity();
	mutable bool m_dirty{ true };
};