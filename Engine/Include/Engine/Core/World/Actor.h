#pragma once

#include <vector>
#include <memory>
#include <type_traits>

class World;
class Level;

class ActorComponent;
class TransformComponent;
class RendererComponent;

class RenderQueue;
class CameraComponent2D;

/*
    Actor
    -------------------------------------------------
    - 정책 없는 Gameplay 컨테이너
    - World / UI Actor의 공통 베이스
*/
class Actor
{
public:
    Actor();
    virtual ~Actor();

    Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

public:
    // =====================================================
    // Identity
    // =====================================================
    virtual bool IsUIActor() const = 0;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    virtual void OnSpawned() {}
    virtual void OnBeginPlay() {}
    virtual void Tick(float deltaTime);
    virtual void OnDestroyed() {}

    void Destroy();
    bool IsPendingDestroy() const { return m_pendingDestroy; }

public:
    // =====================================================
    // World / Level
    // =====================================================
    World* GetWorld() const { return m_world; }
    Level* GetLevel() const { return m_level; }

    virtual bool IsPersistentActor() const { return false; }

protected:
    friend class Level;
    void SetWorld(World* world) { m_world = world; }
    void SetLevel(Level* level) { m_level = level; }

public:
    // =====================================================
    // Component System
    // =====================================================
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<ActorComponent, T>,
            "T must derive from ActorComponent");

        auto comp = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* rawPtr = comp.get();

        m_components.push_back(std::move(comp));
        rawPtr->OnRegister();

        return rawPtr;
    }

    template <typename T>
    T* GetComponent() const
    {
        for (auto& comp : m_components)
        {
            if (auto* casted = dynamic_cast<T*>(comp.get()))
                return casted;
        }
        return nullptr;
    }

    const std::vector<std::unique_ptr<ActorComponent>>&
        GetComponents() const { return m_components; }

public:
    // =====================================================
    // Transform
    // =====================================================
    TransformComponent* GetRootTransform() const { return m_rootTransform; }

protected:
    void SetRootTransform(TransformComponent* root);

public:
    // =====================================================
    // Rendering
    // =====================================================
    void SubmitRenderCommands(
        RenderQueue& queue,
        CameraComponent2D& activeCamera);

protected:
    World* m_world{ nullptr };
    Level* m_level{ nullptr };

    bool m_pendingDestroy{ false };

    std::vector<std::unique_ptr<ActorComponent>> m_components;
    TransformComponent* m_rootTransform{ nullptr };
};
