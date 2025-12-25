#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>

#include "Engine/Core/Component/FAttachmentTransformRules.h"

// Forward declarations
class ActorComponent;
class SceneComponent;
class RendererComponent;
class World;
class Level;

/*
    Actor
    -------------------------------------------------
    - World에 존재하는 게임 오브젝트
    - Component 소유
    - Level에 의해 수명 관리됨
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
    // Lifecycle
    // =====================================================
    virtual void OnSpawned() {}
    virtual void BeginPlay() {}
    virtual void Tick(float deltaTime);
    virtual void OnDestroyed() {}

    void Destroy();
    bool IsPendingDestroy() const { return m_pendingDestroy; }

public:
    // =====================================================
    // Level Ownership Policy
    // =====================================================
    virtual bool IsPersistentActor() const { return false; }
    virtual bool IsUIActor() const { return false; }

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

        comp->SetWorld(m_world);

        // SceneComponent attach
        if constexpr (std::is_base_of_v<SceneComponent, T>)
        {
            if (m_rootComponent && rawPtr != m_rootComponent)
            {
                static_cast<SceneComponent*>(rawPtr)
                    ->AttachTo(m_rootComponent,
                        FAttachmentTransformRules::KeepRelativeTransform);
            }
        }
        // RendererComponent attach (default: root)
        else if constexpr (std::is_base_of_v<RendererComponent, T>)
        {
            if (m_rootComponent)
            {
                static_cast<RendererComponent*>(rawPtr)
                    ->SetAttachComponent(m_rootComponent);
            }
        }

        m_components.push_back(std::move(comp));
        rawPtr->OnRegister();

        return rawPtr;
    }

    template <typename T>
    T* GetComponent() const
    {
        for (auto& comp : m_components)
        {
            if (auto casted = dynamic_cast<T*>(comp.get()))
                return casted;
        }
        return nullptr;
    }

    const std::vector<std::unique_ptr<ActorComponent>>&
        GetComponents() const { return m_components; }

public:
    // =====================================================
    // Root Component
    // =====================================================
    SceneComponent* GetRootComponent() const { return m_rootComponent; }
    void SetRootComponent(SceneComponent* newRoot);

public:
    // =====================================================
    // Renderer Attachment
    // =====================================================
    void AttachRendererTo(
        RendererComponent* renderer,
        SceneComponent* scene);

public:
    // =====================================================
    // Actor Hierarchy
    // =====================================================
    Actor* GetParentActor() const { return m_parentActor; }
    const std::vector<Actor*>& GetChildActors() const { return m_childActors; }

    void AttachToActor(
        Actor* parent,
        const FAttachmentTransformRules& rules =
        FAttachmentTransformRules::KeepWorldTransform);

    void DetachFromParent(
        const FDetachmentTransformRules& rules =
        FDetachmentTransformRules::KeepWorldTransform);

public:
    // =====================================================
    // World
    // =====================================================
    World* GetWorld() const { return m_world; }

private:
    friend class World;
    friend class Level;
    void SetWorld(World* world);

protected:
    SceneComponent* m_rootComponent{ nullptr };

    std::vector<std::unique_ptr<ActorComponent>> m_components;
    std::vector<Actor*> m_childActors;

    Actor* m_parentActor{ nullptr };
    World* m_world{ nullptr };

    bool m_pendingDestroy{ false };
};
