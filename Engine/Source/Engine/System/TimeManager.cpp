#include "Engine/Core/EnginePCH.h"
#include "Engine/System/TimeManager.h"

TimeManager* TimeManager::Instance = nullptr;

void TimeManager::Create()
{
	if (!Instance)
		Instance = new TimeManager();
}

void TimeManager::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

TimeManager& TimeManager::Get()
{
	assert(Instance && "TimeManager::Create() was not called");
	return *Instance;
}

void TimeManager::Update(uint64 currentTickMs)
{
	// 첫 프레임
	if (m_prevTick == 0)
	{
		m_prevTick = currentTickMs;
		return;
	}

	uint64 deltaMs = currentTickMs - m_prevTick;
	m_prevTick = currentTickMs;

	m_deltaTime = static_cast<float>(deltaMs) / 1000.0f;

	// FPS 계산
	m_frameCounter++;
	if (currentTickMs - m_prevFPSCalcTick >= 1000)
	{
		m_prevFPSCalcTick = currentTickMs;
		m_fps = m_frameCounter;
		m_frameCounter = 0;
	}

	// Unscaled time
	m_unscaledTotalTime += m_deltaTime;

	if (m_paused)
		return;

	// Scaled time
	float scaledDt = m_deltaTime * m_timeScale;
	m_totalTime += scaledDt;

	// FixedUpdate 합산
	m_accumulator += scaledDt;
}

void TimeManager::SetTimeScale(float scale)
{
	m_timeScale = std::max(0.0f, scale);
}

bool TimeManager::ShouldRunFixedUpdate() const
{
	return (m_accumulator >= m_fixedDelta);
}

void TimeManager::ConsumeFixedUpdateStep()
{
	m_accumulator -= m_fixedDelta;
}
