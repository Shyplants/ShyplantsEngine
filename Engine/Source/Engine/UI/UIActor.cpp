#include "Engine/PCH/EnginePCH.h"

#include "Engine/UI/UIActor.h"
#include "Engine/UI/Component/UIScreenRootComponent.h"
#include "Engine/UI/Component/UITransformComponent.h"

// =====================================================
// Constructor
// =====================================================

UIActor::UIActor() = default;

// =====================================================
// Lifecycle
// =====================================================

void UIActor::OnSpawned()
{
    Actor::OnSpawned();

    // -------------------------------------------------
    // UIScreenRoot (must be first)
    // -------------------------------------------------
    m_screenRoot = AddComponent<UIScreenRootComponent>();
    SP_ASSERT(m_screenRoot != nullptr);

    // -------------------------------------------------
    // Root UI Transform
    // -------------------------------------------------
    m_rootUITransform = AddComponent<UITransformComponent>();
    SP_ASSERT(m_rootUITransform != nullptr);

    SetRootTransform(m_rootUITransform);

    // -------------------------------------------------
    // Default UISpace
    // -------------------------------------------------
    m_uiSpace.SetViewportSize(0, 0);
    m_uiSpace.SetDPIScale(1.0f);
    m_uiSpace.SetSafeArea(UISafeArea{});

    // UIScreenRootComponent가 참조할 UISpace 연결
    m_screenRoot->UpdateUISpace(
        m_uiSpace.ViewportW,
        m_uiSpace.ViewportH,
        m_uiSpace.DPI,
        m_uiSpace.SafeArea);
}

// =====================================================
// UI Space
// =====================================================

const UISpaceContext& UIActor::GetUISpaceContext() const
{
    return m_uiSpace;
}

UISpaceContext& UIActor::GetUISpaceContextMutable()
{
    return m_uiSpace;
}

void UIActor::UpdateUISpace(
    uint32 viewportW,
    uint32 viewportH,
    float dpi,
    const UISafeArea& safeArea)
{
    bool changed = false;

    changed |= m_uiSpace.SetViewportSize(viewportW, viewportH);
    changed |= m_uiSpace.SetDPIScale(dpi);
    changed |= m_uiSpace.SetSafeArea(safeArea);

    if (changed && m_screenRoot)
    {
        m_screenRoot->UpdateUISpace(
            viewportW,
            viewportH,
            dpi,
            safeArea);
    }
}
