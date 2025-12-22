#include "Engine/Core/Time/TimeSystem.h"

#include <algorithm>

void TimeSystem::Update(double platformTimeSeconds)
{
    if (m_lastPlatformTime == 0.0)
    {
        // First frame bootstrap
        m_lastPlatformTime = platformTimeSeconds;
        return;
    }

    double rawDelta = platformTimeSeconds - m_lastPlatformTime;
    m_lastPlatformTime = platformTimeSeconds;

    // Clamp for safety (debug break / hitch protection)
    rawDelta = std::clamp(rawDelta, 0.0, 0.25);

    m_unscaledDeltaTime = static_cast<float>(rawDelta);

    if (m_paused)
    {
        m_deltaTime = 0.0f;
    }
    else
    {
        m_deltaTime = m_unscaledDeltaTime * m_timeScale;
        m_elapsedTime += m_deltaTime;
    }

    ++m_frameCount;
}

void TimeSystem::SetTimeScale(float scale)
{
    m_timeScale = std::max(scale, 0.0f);
}

void TimeSystem::Pause()
{
    m_paused = true;
}

void TimeSystem::Resume()
{
    m_paused = false;
}
