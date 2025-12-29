#include "Engine/PCH/EnginePCH.h"
#include "Engine/Core/Component/TransformComponent.h"

TransformComponent::TransformComponent(Actor* owner)
    : ActorComponent(owner)
{
}

TransformComponent::~TransformComponent() = default;

const std::vector<TransformComponent*>&
TransformComponent::GetChildren() const
{
    return m_children;
}

void TransformComponent::AttachTo(TransformComponent* parent)
{
#if defined(_DEBUG)
    SP_ASSERT(parent);
    SP_ASSERT(parent != this);
#endif
    if (m_parent == parent)
        return;

    if (m_parent)
        Detach();

    m_parent = parent;
    parent->AddChild(this);

    MarkDirtyRecursive();
    OnAttached(parent);
}

void TransformComponent::Detach()
{
    if (!m_parent)
        return;

    TransformComponent* oldParent = m_parent;
    m_parent->RemoveChild(this);
    m_parent = nullptr;

    MarkDirtyRecursive();
    OnDetached();
}

void TransformComponent::AddChild(TransformComponent* child)
{
    if (std::find(m_children.begin(), m_children.end(), child)
        == m_children.end())
    {
        m_children.push_back(child);
    }
}

void TransformComponent::RemoveChild(TransformComponent* child)
{
    m_children.erase(
        std::remove(m_children.begin(), m_children.end(), child),
        m_children.end());
}

void TransformComponent::MarkDirtyRecursive()
{
    OnTransformDirty();

    for (TransformComponent* child : m_children)
    {
        child->MarkDirtyRecursive();
    }
}
