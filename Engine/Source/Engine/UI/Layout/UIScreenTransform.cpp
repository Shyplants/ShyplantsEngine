#include "Engine/UI/Layout/UIScreenTransform.h"

#include "Engine/UI/Components/UIScreenRootComponent.h"
#include "Engine/UI/Layout/UIAnchorResolver.h"

using namespace DirectX;

// =====================================================
// Constructor
// =====================================================

UIScreenTransform::UIScreenTransform(Actor* owner)
    : UIComponent(owner)
{
}

UIScreenTransform::~UIScreenTransform() = default;

// =====================================================
// Lifecycle
// =====================================================

void UIScreenTransform::OnRegister()
{
    UIComponent::OnRegister();

    if (m_root)
    {
        m_root->RegisterChild(this);
        m_dirty = true;
    }
}

void UIScreenTransform::OnUnregister()
{
    if (m_root)
    {
        m_root->UnregisterChild(this);
    }

    UIComponent::OnUnregister();
}

// =====================================================
// Root binding
// =====================================================

void UIScreenTransform::SetRoot(UIScreenRootComponent* root)
{
    if (m_root == root)
        return;

    // 이미 등록된 상태라면 명시적으로 해제
    if (IsActive() && m_root)
    {
        m_root->UnregisterChild(this);
    }

    m_root = root;

    if (IsActive() && m_root)
    {
        m_root->RegisterChild(this);
        m_dirty = true;
    }
}

// =====================================================
// Layout
// =====================================================

void UIScreenTransform::SetAnchor(const UIAnchorData& anchor)
{
    m_anchor = anchor;
    m_dirty = true;
}

void UIScreenTransform::SetLocalOffset(float x, float y)
{
    m_localOffset = { x, y };
    m_dirty = true;
}

void UIScreenTransform::MarkLayoutDirty()
{
    m_dirty = true;
}

XMFLOAT2 UIScreenTransform::GetScreenPosition() const
{
    if (m_dirty)
        Recalculate();

    return m_cachedPos;
}

// =====================================================
// Internal
// =====================================================

void UIScreenTransform::Recalculate() const
{
    if (!m_root)
        return;

    const auto& space = m_root->GetUISpaceContext();

    XMFLOAT2 base =
        UIAnchorResolver::Resolve(m_anchor, space);

    m_cachedPos = {
        base.x + m_localOffset.x,
        base.y + m_localOffset.y
    };

    m_dirty = false;
}
