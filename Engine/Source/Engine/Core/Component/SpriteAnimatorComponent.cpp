#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Component/SpriteAnimatorComponent.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Graphics/Material/MaterialInstance.h"
#include "Engine/Core/World/Actor.h"

// =========================================================
// Constructor
// =========================================================

SpriteAnimatorComponent::SpriteAnimatorComponent(Actor* owner)
    : ActorComponent(owner)
{
}

// =========================================================
// Setup
// =========================================================

void SpriteAnimatorComponent::OnRegister()
{
    ActorComponent::OnRegister();

    // SpriteRendererComponent 자동 연결
    m_spriteRenderer = GetOwner()->GetComponent<SpriteRendererComponent>();
    if (!m_spriteRenderer)
    {
        SP_LOG(LogGame, ELogLevel::Warning,
            "SpriteAnimatorComponent requires SpriteRendererComponent");
        return;
    }

    m_materialInstance = m_spriteRenderer->GetMaterialInstance();
    if (!m_materialInstance)
    {
        SP_LOG(LogGame, ELogLevel::Warning,
            "SpriteRendererComponent has no MaterialInstance");
        return;
    }
}

// =========================================================
// Animation Control
// =========================================================

void SpriteAnimatorComponent::AddClip(const SpriteAnimationClip& clip)
{
    SP_ASSERT(!clip.frames.empty());
    m_clips[clip.name] = clip;
}

void SpriteAnimatorComponent::Play(const std::wstring& clipName, bool restart)
{
    auto it = m_clips.find(clipName);
    if (it == m_clips.end())
        return;

    if (m_currentClip == &it->second && !restart)
        return;

    m_currentClip = &it->second;
    m_accumulatedTime = 0.0f;
    m_frameIndex = 0;

    // 첫 프레임 즉시 반영
    if (m_materialInstance && !m_currentClip->frames.empty())
    {
        m_materialInstance->SetSourceRect(
            m_currentClip->frames[0]);
    }
}

void SpriteAnimatorComponent::Stop()
{
    m_currentClip = nullptr;
    m_accumulatedTime = 0.0f;
    m_frameIndex = 0;
}

// =========================================================
// Tick
// =========================================================

void SpriteAnimatorComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    if (!m_currentClip || !m_materialInstance)
        return;

    const float frameTime = 1.0f / m_currentClip->fps;
    m_accumulatedTime += deltaTime;

    while (m_accumulatedTime >= frameTime)
    {
        m_accumulatedTime -= frameTime;
        AdvanceFrame();
    }
}

// =========================================================
// Internal
// =========================================================

void SpriteAnimatorComponent::AdvanceFrame()
{
    SP_ASSERT(m_currentClip);

    ++m_frameIndex;

    if (m_frameIndex >= m_currentClip->frames.size())
    {
        if (m_currentClip->loop)
        {
            m_frameIndex = 0;
        }
        else
        {
            m_frameIndex = static_cast<uint32>(m_currentClip->frames.size() - 1);
        }
    }

    m_materialInstance->SetSourceRect(
        m_currentClip->frames[m_frameIndex]);
}
