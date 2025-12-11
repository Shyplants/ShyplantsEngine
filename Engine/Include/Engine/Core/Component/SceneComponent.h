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
	// Transform Set/Get
	void SetLocalPosition(const DirectX::XMFLOAT3& pos);
	void SetLocalRotation(const DirectX::XMFLOAT3& rot);
	void SetLocalScale(const DirectX::XMFLOAT3& scale);

	DirectX::XMFLOAT3 GetLocalPosition() const { return m_localPosition; }
	DirectX::XMFLOAT3 GetLocalRotation() const { return m_localRotation; }
	DirectX::XMFLOAT3 GetLocalScale() const { return m_localScale; }

	// Transform Query
	DirectX::XMFLOAT3 GetWorldPosition();
	DirectX::XMFLOAT3 GetWorldRotation();
	DirectX::XMFLOAT3 GetWorldScale();
	
	DirectX::XMMATRIX GetLocalMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	// Attachment
	void AttachTo(SceneComponent* parent, const FAttachmentTransformRules& rules = FAttachmentTransformRules::KeepWorldTransform);
	void Detach(const FDetachmentTransformRules& rules = FDetachmentTransformRules::KeepWorldTransform);
	
	SceneComponent* GetParent() const { return m_parent; }
	const std::vector<SceneComponent*>& GetChildren() const { return m_children; }

	// Internal
	void AddChild(SceneComponent* child);
	void RemoveChild(SceneComponent* child);

private:
	void MarkDirtyRecursive();

private:
	SceneComponent* m_parent{ nullptr };
	std::vector<SceneComponent*> m_children;

	// Local Transform
	DirectX::XMFLOAT3 m_localPosition{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_localRotation{ 0.0f, 0.0f, 0.0f }; // 오일러 각도
	DirectX::XMFLOAT3 m_localScale{ 1.0f, 1.0f, 1.0f };
	
	// Cached World Transform
	mutable DirectX::XMMATRIX m_cachedWorldMatrix = DirectX::XMMatrixIdentity();
	mutable bool m_worldDirty{ true };
};