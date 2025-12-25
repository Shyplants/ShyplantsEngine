#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;

// =====================================================
// Constructor / Destructor
// =====================================================

SceneComponent::SceneComponent(Actor* owner)
    : ActorComponent(owner)
{
}

SceneComponent::~SceneComponent() = default;

// =====================================================
// Local Transform Setters
// =====================================================

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

// =====================================================
// World Queries
// =====================================================

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
    if (!m_parent)
        return m_localPosition;

    const XMMATRIX& world = GetWorldMatrix();

    return {
        world.r[3].m128_f32[0],
        world.r[3].m128_f32[1],
        world.r[3].m128_f32[2]
    };
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

// =====================================================
// Matrix Builders
// =====================================================

XMMATRIX SceneComponent::GetLocalMatrix() const
{
    const XMMATRIX S = XMMatrixScaling(
        m_localScale.x,
        m_localScale.y,
        m_localScale.z);

    const XMMATRIX R = XMMatrixRotationRollPitchYaw(
        XMConvertToRadians(m_localRotation.x),
        XMConvertToRadians(m_localRotation.y),
        XMConvertToRadians(m_localRotation.z));

    const XMMATRIX T = XMMatrixTranslation(
        m_localPosition.x,
        m_localPosition.y,
        m_localPosition.z);

    return S * R * T;
}

XMMATRIX SceneComponent::GetWorldMatrix() const
{
    if (!m_worldDirty)
        return m_cachedWorldMatrix;

    m_cachedWorldMatrix =
        m_parent
        ? m_parent->GetWorldMatrix() * GetLocalMatrix()
        : GetLocalMatrix();

    m_worldDirty = false;
    return m_cachedWorldMatrix;
}

// =====================================================
// Attachment (PUBLIC / SAFE)
// =====================================================

void SceneComponent::AttachTo(
    SceneComponent* parent,
    const FAttachmentTransformRules& rules)
{
    SP_ASSERT(parent);
    SP_ASSERT(parent != this);
    SP_ASSERT(parent->GetOwner() == GetOwner());

    AttachToUnsafe(parent, rules);
}

// =====================================================
// Attachment (INTERNAL / UNSAFE)
// =====================================================

void SceneComponent::AttachToUnsafe(
    SceneComponent* parent,
    const FAttachmentTransformRules& rules)
{
    SceneComponent* oldParent = m_parent;
    XMMATRIX oldWorld = GetWorldMatrix();

    if (m_parent)
        m_parent->RemoveChild(this);

    m_parent = parent;
    parent->AddChild(this);

    if (rules.bMaintainWorld)
    {
        XMMATRIX invParent =
            XMMatrixInverse(nullptr, parent->GetWorldMatrix());

        XMMATRIX newLocal = oldWorld * invParent;

        XMVECTOR scaleV, rotQ, transV;
        XMMatrixDecompose(&scaleV, &rotQ, &transV, newLocal);

        XMStoreFloat3(&m_localScale, scaleV);
        XMStoreFloat3(&m_localPosition, transV);
        // rotation intentionally preserved (Euler)
    }

    MarkDirtyRecursive();

    // ---- Attachment Event (AFTER state change)
    OnAttached(parent);
}

void SceneComponent::DetachUnsafe(
    const FDetachmentTransformRules& rules)
{
    if (!m_parent)
        return;

    SceneComponent* oldParent = m_parent;
    XMMATRIX oldWorld = GetWorldMatrix();

    m_parent->RemoveChild(this);
    m_parent = nullptr;

    if (rules.bMaintainWorld)
    {
        XMVECTOR scaleV, rotQ, transV;
        XMMatrixDecompose(&scaleV, &rotQ, &transV, oldWorld);

        XMStoreFloat3(&m_localScale, scaleV);
        XMStoreFloat3(&m_localPosition, transV);
    }

    MarkDirtyRecursive();

    // ---- Detachment Event (AFTER state change)
    OnDetached(oldParent);
}

// =====================================================
// Children
// =====================================================

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

// =====================================================
// Dirty Propagation
// =====================================================

void SceneComponent::MarkDirtyRecursive()
{
    m_worldDirty = true;
    OnTransformDirty();

    for (SceneComponent* child : m_children)
    {
        child->MarkDirtyRecursive();
    }
}
