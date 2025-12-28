#pragma once

#include <vector>

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/UI/UISpaceContext.h"

// Forward declarations
class UIScreenTransform;

/*
    UIScreenRootComponent
    -------------------------------------------------
    - Screen-space UI 트리의 Root
    - World Transform과 무관
    - UISpaceContext(Viewport / DPI / SafeArea) 관리
    - 하위 UIScreenTransform Dirty 전파 책임
*/
class UIScreenRootComponent final : public SceneComponent
{
public:
    explicit UIScreenRootComponent(Actor* owner);

public:
    // =====================================================
    // UI Space
    // =====================================================
    const UISpaceContext& GetUISpaceContext() const;

    void UpdateUISpace(
        uint32 viewportW,
        uint32 viewportH,
        float dpi,
        const UISafeArea& safeArea);

public:
    // =====================================================
    // UI Tree management
    // =====================================================
    void RegisterChild(UIScreenTransform* child);
    void UnregisterChild(UIScreenTransform* child);

private:
    void MarkChildrenDirty();

private:
    UISpaceContext m_space;
    std::vector<UIScreenTransform*> m_children;
};
