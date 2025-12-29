#pragma once

#include "Engine/Core/World/Actor.h"
#include "Engine/UI/UISpaceContext.h"

// Forward declarations
class UIScreenRootComponent;
class UITransformComponent;

/*
    UIActor
    -------------------------------------------------
    - Screen-space 전용 Actor
    - 모든 UI 요소의 루트
    - UISpaceContext 소유
*/
class UIActor final : public Actor
{
public:
    UIActor();
    ~UIActor() override = default;

public:
    // =====================================================
    // Identity
    // =====================================================
    bool IsUIActor() const override { return true; }
    bool IsPersistentActor() const override { return true; }

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    void OnSpawned() override;

public:
    // =====================================================
    // UI Space
    // =====================================================
    const UISpaceContext& GetUISpaceContext() const;
    UISpaceContext& GetUISpaceContextMutable();

    void UpdateUISpace(
        uint32 viewportW,
        uint32 viewportH,
        float dpi,
        const UISafeArea& safeArea);

public:
    // =====================================================
    // Root access
    // =====================================================
    UIScreenRootComponent* GetUIScreenRoot() const
    {
        return m_screenRoot;
    }

    UITransformComponent* GetRootUITransform() const
    {
        return m_rootUITransform;
    }

private:
    // -------------------------------------------------
    // Components
    // -------------------------------------------------
    UIScreenRootComponent* m_screenRoot{ nullptr };
    UITransformComponent* m_rootUITransform{ nullptr };

    // -------------------------------------------------
    // UI Space
    // -------------------------------------------------
    UISpaceContext m_uiSpace;
};
