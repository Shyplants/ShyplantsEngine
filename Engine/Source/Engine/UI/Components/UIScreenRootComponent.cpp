#include "Engine/UI/Components/UIScreenRootComponent.h"

#include <algorithm>

#include "Engine/UI/Layout/UIScreenTransform.h"

// =====================================================
// Constructor
// =====================================================

UIScreenRootComponent::UIScreenRootComponent(Actor* owner)
    : SceneComponent(owner)
{
}

// =====================================================
// UISpace
// =====================================================

const UISpaceContext& UIScreenRootComponent::GetUISpaceContext() const
{
    return m_space;
}

void UIScreenRootComponent::UpdateUISpace(
    uint32 viewportW,
    uint32 viewportH,
    float dpi,
    const UISafeArea& safeArea)
{
    bool changed = false;

    changed |= m_space.SetViewportSize(viewportW, viewportH);
    changed |= m_space.SetDPIScale(dpi);
    changed |= m_space.SetSafeArea(safeArea);

    if (changed)
        MarkChildrenDirty();
}

// =====================================================
// UI Tree management
// =====================================================

void UIScreenRootComponent::RegisterChild(UIScreenTransform* child)
{
    if (!child)
        return;

    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
        return;

    m_children.push_back(child);
}

void UIScreenRootComponent::UnregisterChild(UIScreenTransform* child)
{
    auto it = std::remove(m_children.begin(), m_children.end(), child);
    if (it != m_children.end())
        m_children.erase(it, m_children.end());
}

// =====================================================
// Internal
// =====================================================

void UIScreenRootComponent::MarkChildrenDirty()
{
    for (UIScreenTransform* child : m_children)
    {
        if (child)
            child->MarkLayoutDirty();
    }
}
