#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Graphics/Animation/SpriteAnimationClip.h"
#include "Common/Types.h"

#include <unordered_map>
#include <string>
#include <vector>

class SpriteRendererComponent;
class MaterialInstance;

class SpriteAnimatorComponent final : public ActorComponent
{
public:
    explicit SpriteAnimatorComponent(Actor* owner);
    ~SpriteAnimatorComponent() override = default;

public:
    // =========================================================
    // Animation Control
    // =========================================================
    void AddClip(const SpriteAnimationClip& clip);
    void Play(const std::wstring& clipName, bool restart = true);
    void Stop();

    bool IsPlaying() const { return m_currentClip != nullptr; }

public:
    // =========================================================
    // ActorComponent Interface
    // =========================================================
    void OnRegister() override;
    void Tick(float deltaTime) override;

private:
    void AdvanceFrame();

private:
    // =========================================================
    // Cached References
    // =========================================================
    SpriteRendererComponent* m_spriteRenderer{ nullptr };
    MaterialInstance* m_materialInstance{ nullptr };

private:
    // =========================================================
    // Animation State
    // =========================================================
    std::unordered_map<std::wstring, SpriteAnimationClip> m_clips;
    const SpriteAnimationClip* m_currentClip{ nullptr };

    float  m_accumulatedTime{ 0.0f };
    uint32 m_frameIndex{ 0 };
};