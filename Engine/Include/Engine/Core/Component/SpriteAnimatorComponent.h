#pragma once

#include "Engine/Core/Component/ActorComponent.h"
#include "Engine/Graphics/Animation/SpriteAnimationClip.h"
#include "Common/Types.h"
#include <unordered_map>


class SpriteRendererComponent;

class SpriteAnimatorComponent : public ActorComponent
{
public:
	explicit SpriteAnimatorComponent(Actor* owner);
	~SpriteAnimatorComponent() override;

	void Tick(float deltaTime) override;

public:
	void AddClip(const SpriteAnimationClip& clip);

	void Play(const std::wstring& name, bool restart = false);
	void Stop();
	void Pause(bool pause);

	bool IsPlaying() const { return m_isPlaying; }
	bool IsPaused() const { return m_isPaused; }

	const std::wstring& GetCurrentClipName() const { return m_currentClip; }

private:
	void ApplyFrame();

private:
	SpriteRendererComponent* m_spriteRenderer{ nullptr };

	std::unordered_map<std::wstring, SpriteAnimationClip> m_clips;

	std::wstring m_currentClip = L"";
	int32 m_currentFrame{ 0 };

	float m_time{ 0.0f };
	float m_frameDuration{ 0.1f };

	bool m_isPlaying{ false };
	bool m_isPaused{ false };
};