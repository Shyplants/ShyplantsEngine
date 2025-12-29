#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Actor.h"

#include "Engine/UI/Component/UITransformComponent.h"
#include "Engine/UI/Component/UIScreenRootComponent.h"
#include "Engine/UI/Layout/UIAnchorResolver.h"

using namespace DirectX;

// =====================================================
// Constructor
// =====================================================

UITransformComponent::UITransformComponent(Actor* owner)
    : TransformComponent(owner)
{
}

UITransformComponent::~UITransformComponent() = default;

// =====================================================
// ActorComponent lifecycle
// =====================================================

void UITransformComponent::OnRegister()
{
    TransformComponent::OnRegister();
    ResolveRoot();
}

void UITransformComponent::OnUnregister()
{
    if (m_root)
    {
        m_root->UnregisterUITransform(this);
        m_root = nullptr;
    }

    m_space = nullptr;
    TransformComponent::OnUnregister();
}

// =====================================================
// Layout configuration
// =====================================================

void UITransformComponent::SetAnchor(const UIAnchorData& anchor)
{
    m_anchor = anchor;
    MarkLayoutDirty();
}

void UITransformComponent::SetLocalOffset(const XMFLOAT2& offset)
{
    m_localOffset = offset;
    MarkLayoutDirty();
}

void UITransformComponent::SetUISpace(const UISpaceContext* space)
{
    m_space = space;
    MarkLayoutDirty();
}

void UITransformComponent::MarkLayoutDirty()
{
    m_dirty = true;
}

// =====================================================
// TransformComponent hook
// =====================================================

void UITransformComponent::OnTransformDirty()
{
    MarkLayoutDirty();
}

// =====================================================
// Transform queries
// =====================================================

XMMATRIX UITransformComponent::GetWorldMatrix() const
{
    if (m_dirty)
        Recalculate();

    return XMMatrixTranslation(
        m_cachedPos.x,
        m_cachedPos.y,
        0.0f);
}

XMFLOAT3 UITransformComponent::GetWorldPositionFast() const
{
    if (m_dirty)
        Recalculate();

    return { m_cachedPos.x, m_cachedPos.y, 0.0f };
}

// =====================================================
// Internal
// =====================================================

void UITransformComponent::ResolveRoot()
{
    if (!GetOwner())
        return;

    m_root = GetOwner()->GetComponent<UIScreenRootComponent>();

#if defined(_DEBUG)
    SP_ASSERT(m_root != nullptr);
#endif

    if (m_root)
    {
        m_root->RegisterUITransform(this);
    }
}

void UITransformComponent::Recalculate() const
{
    if (!m_space)
        return;

    XMFLOAT2 base =
        UIAnchorResolver::Resolve(m_anchor, *m_space);

    m_cachedPos = {
        base.x + m_localOffset.x,
        base.y + m_localOffset.y
    };

    m_dirty = false;
}
