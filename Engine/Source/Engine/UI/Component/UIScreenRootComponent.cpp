#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/Actor.h"

#include "Engine/UI/Component/UIScreenRootComponent.h"
#include "Engine/UI/Component/UITransformComponent.h"

// =====================================================
// Constructor
// =====================================================

UIScreenRootComponent::UIScreenRootComponent(Actor* owner)
    : ActorComponent(owner)
{
}

UIScreenRootComponent::~UIScreenRootComponent() = default;

// =====================================================
// UISpace
// =====================================================

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
    {
        MarkAllUITransformsDirty();
    }
}

// =====================================================
// Registration
// =====================================================

void UIScreenRootComponent::RegisterUITransform(
    UITransformComponent* transform)
{
#if defined(_DEBUG)
    SP_ASSERT(transform != nullptr);
#endif

    if (std::find(
        m_uiTransforms.begin(),
        m_uiTransforms.end(),
        transform) != m_uiTransforms.end())
    {
        return;
    }

    m_uiTransforms.push_back(transform);
    transform->SetUISpace(&m_space);
}

void UIScreenRootComponent::UnregisterUITransform(
    UITransformComponent* transform)
{
    if (!transform)
        return;

    // ---- 중요: dangling reference 방지
    transform->SetUISpace(nullptr);

    m_uiTransforms.erase(
        std::remove(
            m_uiTransforms.begin(),
            m_uiTransforms.end(),
            transform),
        m_uiTransforms.end());
}

// =====================================================
// Internal
// =====================================================

void UIScreenRootComponent::MarkAllUITransformsDirty()
{
    for (UITransformComponent* ui : m_uiTransforms)
    {
        if (ui)
        {
            ui->MarkLayoutDirty();
        }
    }
}
