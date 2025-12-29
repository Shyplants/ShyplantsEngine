#pragma once

#include <vector>
#include <algorithm>

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/UI/UISpaceContext.h"

// Forward declarations
class UITransformComponent;

/*
    UIScreenRootComponent
    -------------------------------------------------
    - UI Space (viewport / dpi / safe area) 소유자
    - UITransformComponent 등록 및 Dirty 전파 담당
*/
class UIScreenRootComponent final : public ActorComponent
{
public:
    explicit UIScreenRootComponent(Actor* owner);
    ~UIScreenRootComponent() override;

public:
    // =====================================================
    // UISpace
    // =====================================================
    const UISpaceContext& GetUISpace() const { return m_space; }

    void UpdateUISpace(
        uint32 viewportW,
        uint32 viewportH,
        float dpi,
        const UISafeArea& safeArea);

public:
    // =====================================================
    // Registration
    // =====================================================
    void RegisterUITransform(UITransformComponent* transform);
    void UnregisterUITransform(UITransformComponent* transform);

private:
    void MarkAllUITransformsDirty();

private:
    UISpaceContext m_space;
    std::vector<UITransformComponent*> m_uiTransforms;
};
