#include "Engine/Core/EnginePCH.h"
#include "Engine/Core/Component/SpriteAnimatorComponent.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"

#include "Engine/Core/World/Actor.h"

SpriteAnimatorComponent::SpriteAnimatorComponent(Actor* owner)
	: ActorComponent(owner)
{
}

SpriteAnimatorComponent::~SpriteAnimatorComponent()
{
}

void SpriteAnimatorComponent::Tick(float deltaTime)
{
	if (!m_isPlaying || m_isPaused)
		return;

	auto it = m_clips.find(m_currentClip);
	if (it == m_clips.end())
		return;

	const auto& clip = it->second;

	m_time += deltaTime;

	if (m_time >= m_frameDuration)
	{
		m_time -= m_frameDuration;
		m_currentFrame++;

		if (m_currentFrame >= static_cast<int32>(clip.frames.size()))
		{
			if (clip.loop)
				m_currentFrame = 0;
			else
			{
				m_currentFrame = clip.frames.size() - 1;
				m_isPlaying = false;
			}
		}

		ApplyFrame();
	}
}

void SpriteAnimatorComponent::AddClip(const SpriteAnimationClip& clip)
{
	if (!clip.IsValid())
		return;

	m_clips[clip.name] = clip;
}

void SpriteAnimatorComponent::Play(const std::wstring& name, bool restart)
{
	auto it = m_clips.find(name);
	if (it == m_clips.end())
		return;

	if (!restart && m_isPlaying && m_currentClip == name)
		return;

	m_currentClip = name;
	m_currentFrame = 0;
	m_time = 0.0f;

	const auto& clip = it->second;
	assert(clip.fps > 0.0f);

	m_frameDuration = 1.0f / clip.fps;

	m_isPlaying = true;
	m_isPaused = false;

	if (!m_spriteRenderer)
		m_spriteRenderer = GetOwner()->GetComponent<SpriteRendererComponent>();

	ApplyFrame();
}

void SpriteAnimatorComponent::Stop()
{
	m_isPlaying = false;
	m_isPaused = false;
}

void SpriteAnimatorComponent::Pause(bool pause)
{
	m_isPaused = pause;
}

void SpriteAnimatorComponent::ApplyFrame()
{
	if (!m_spriteRenderer)
		return;

	auto it = m_clips.find(m_currentClip);
	if (it == m_clips.end())
		return;

	const auto& clip = it->second;

	if (m_currentFrame < 0 || m_currentFrame >= static_cast<int32>(clip.frames.size()))
		return;

	m_spriteRenderer->SetRect(clip.frames[m_currentFrame]);
}
