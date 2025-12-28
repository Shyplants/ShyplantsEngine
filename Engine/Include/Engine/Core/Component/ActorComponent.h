#pragma once

#include "Common/Types.h"

// Forward declarations
class Actor;
class World;

/*
    ActorComponent
    -------------------------------------------------
    - Actor에 부착되는 모든 컴포넌트의 최상위 베이스
    - 수명은 Actor에 의해 관리됨
    - Transform / Render / UI 책임 없음
*/
class ActorComponent
{
public:
    explicit ActorComponent(Actor* owner);
    virtual ~ActorComponent();

    ActorComponent(const ActorComponent&) = delete;
    ActorComponent& operator=(const ActorComponent&) = delete;

public:
    // =====================================================
    // Lifecycle (Actor-driven)
    // =====================================================
    virtual void OnRegister();
    virtual void OnUnregister();

    virtual void OnBeginPlay();
    virtual void OnEndPlay();

    virtual void Tick(float /*deltaTime*/) {}

public:
    // =====================================================
    // State
    // =====================================================
    bool IsActive() const { return m_active; }
    void SetActive(bool active);

    bool IsTickEnabled() const { return m_tickEnabled; }
    void SetTickEnabled(bool enable) { m_tickEnabled = enable; }

public:
    // =====================================================
    // Ownership
    // =====================================================
    Actor* GetOwner() const { return m_owner; }
    World* GetWorld() const;

protected:
    // =====================================================
    // Internal hooks
    // =====================================================
    virtual void OnActivated() {}
    virtual void OnDeactivated() {}

protected:
    Actor* m_owner{ nullptr };

    bool m_active{ true };
    bool m_tickEnabled{ false };
};
