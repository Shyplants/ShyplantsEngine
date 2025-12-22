#pragma once

class Actor;
class World;

/*
    ActorComponent
    -------------------------------------------------
    - Actor에 부착되는 기본 컴포넌트
    - Tick / 활성 상태 관리
    - 렌더링 책임 없음
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
    // Lifecycle
    // =====================================================
    virtual void OnRegister() {}
    virtual void OnUnregister() {}
    virtual void OnDestroy() {}
    virtual void Tick(float deltaTime) {}

public:
    // =====================================================
    // State
    // =====================================================
    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

    bool IsTickEnabled() const { return m_canTick; }
    void SetTickEnabled(bool enabled) { m_canTick = enabled; }

public:
    // =====================================================
    // Owner / World
    // =====================================================
    Actor* GetOwner() const { return m_owner; }
    World* GetWorld() const;

protected:
    friend class Actor;
    void SetWorld(World* world) { m_world = world; }

protected:
    Actor* m_owner{ nullptr };
    World* m_world{ nullptr };

    bool m_active{ true };
    bool m_canTick{ true };
};
