#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Core/Component/FAttachmentTransformRules.h"
#include "DirectXMath.h"

#include <vector>

class Actor;

class SceneComponent : public ActorComponent
{
public:
	explicit SceneComponent(Actor* owner);
	~SceneComponent() override;

public:
	// ============================
	// Local Transform Setters
	// ============================
	void SetLocalPosition(const DirectX::XMFLOAT3& pos);
	void SetLocalRotation(const DirectX::XMFLOAT3& rot); // Euler (deg)
	void SetLocalScale(const DirectX::XMFLOAT3& scale);

	// ============================
	// Local Transform Getters
	// ============================
	DirectX::XMFLOAT3 GetLocalPosition() const { return m_localPosition; }
	DirectX::XMFLOAT3 GetLocalRotation() const { return m_localRotation; }
	DirectX::XMFLOAT3 GetLocalScale() const { return m_localScale; }

public:
	// ============================
	// World Transform Query
	// ============================
	DirectX::XMFLOAT3 GetWorldPosition();
	DirectX::XMFLOAT3 GetWorldPositionFast() const;
	DirectX::XMFLOAT3 GetWorldScale();

	DirectX::XMMATRIX GetLocalMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

public:
	// ============================
	// Attachment
	// ============================
	void AttachTo(
		SceneComponent* parent,
		const FAttachmentTransformRules& rules =
		FAttachmentTransformRules::KeepRelativeTransform);

	void Detach(
		const FDetachmentTransformRules& rules =
		FDetachmentTransformRules::KeepRelativeTransform);

	SceneComponent* GetParent() const { return m_parent; }
	const std::vector<SceneComponent*>& GetChildren() const { return m_children; }

protected:
	// ============================
	// Internal (Actor Only)
	// ============================
	void AddChild(SceneComponent* child);
	void RemoveChild(SceneComponent* child);

protected:
	// ============================
	// Dirty Notification Hook
	// ============================
	virtual void OnTransformDirty() {}

private:
	void MarkDirtyRecursive();

private:
	// ============================
	// Hierarchy
	// ============================
	SceneComponent* m_parent{ nullptr };
	std::vector<SceneComponent*> m_children;

	// ============================
	// Local Transform (Euler, deg)
	// ============================
	DirectX::XMFLOAT3 m_localPosition{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_localRotation{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_localScale{ 1.0f, 1.0f, 1.0f };

	// ============================
	// Cached World Transform
	// ============================
	mutable DirectX::XMMATRIX m_cachedWorldMatrix =
		DirectX::XMMatrixIdentity();

	mutable bool m_worldDirty{ true };
};
