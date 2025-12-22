#pragma once

#include "Common/Types.h"

/*
    TimeSystem
    -------------------------------------------------
    - Engine-owned global time state
    - Platform-independent
    - Single source of truth for all timing
*/
class TimeSystem
{
public:
    TimeSystem() = default;

    // -------------------------------------------------
    // Update (Engine loop)
    // -------------------------------------------------
    void Update(double platformTimeSeconds);

public:
    // -------------------------------------------------
    // Accessors
    // -------------------------------------------------
    float GetDeltaTime() const { return m_deltaTime; }
    float GetUnscaledDeltaTime() const { return m_unscaledDeltaTime; }

    double GetElapsedTime() const { return m_elapsedTime; }
    uint64 GetFrameCount() const { return m_frameCount; }

public:
    // -------------------------------------------------
    // Time control
    // -------------------------------------------------
    void SetTimeScale(float scale);
    float GetTimeScale() const { return m_timeScale; }

    void Pause();
    void Resume();
    bool IsPaused() const { return m_paused; }

private:
    // -------------------------------------------------
    // Raw platform time
    // -------------------------------------------------
    double m_lastPlatformTime{ 0.0 };

    // -------------------------------------------------
    // Computed times
    // -------------------------------------------------
    float  m_unscaledDeltaTime{ 0.0f };
    float  m_deltaTime{ 0.0f };
    double m_elapsedTime{ 0.0 };

    // -------------------------------------------------
    // State
    // -------------------------------------------------
    float  m_timeScale{ 1.0f };
    bool   m_paused{ false };
    uint64 m_frameCount{ 0 };
};
