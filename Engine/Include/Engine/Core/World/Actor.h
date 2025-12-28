#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>

#include "Engine/Core/Component/FAttachmentTransformRules.h"

// Forward declarations
class World;
class Level;

class ActorComponent;
class SceneComponent;
class RendererComponent;
class CameraComponent2D;

class RenderQueue;

/*
    Actor
    -------------------------------------------------
    - World에 소속되는 Gameplay 단위
    - Component 기반 확장
    - Transform의 Root는 SceneComponent
    - Render / Tick 정책 없음 (조립자 역할)
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

        // SceneComponent → root attach
        if constexpr (std::is_base_of_v<SceneComponent, T>)
        {
            if (m_rootComponent && rawPtr != m_rootComponent)
            {
                static_cast<SceneComponent*>(rawPtr)
                    ->AttachTo(
                        m_rootComponent,
                        FAttachmentTransformRules::KeepRelativeTransform);
            }
        }
        // RendererComponent → default root binding
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
    // Rendering
    // =====================================================
    void SubmitRenderCommands(
        RenderQueue& queue,
        CameraComponent2D& activeCamera);

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

protected:
    World* m_world{ nullptr };
    Level* m_level{ nullptr };

    bool m_pendingDestroy{ false };

    // -------------------------------------------------
    // Components
    // -------------------------------------------------
    std::vector<std::unique_ptr<ActorComponent>> m_components;


    // -------------------------------------------------
    // Scene
    // -------------------------------------------------
    SceneComponent* m_rootComponent{ nullptr };

    
    // -------------------------------------------------
    // Parent / Children
    // -------------------------------------------------
    Actor* m_parentActor{ nullptr };
    std::vector<Actor*> m_childActors;
};
