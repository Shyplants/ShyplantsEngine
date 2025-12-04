#pragma once

#include "Common/Types.h"

class TimeManager
{
public:
	static void Create();
	static void Destroy();
	static TimeManager& Get();

private:
	TimeManager() = default;
	~TimeManager() = default;

public:
	// 매 프레임 시간 업데이트 (Engine::TickOnce 에서 호출)
	void Update(uint64 currentTickMs);

	float GetDeltaTime() const { return m_deltaTime * m_timeScale; }
	float GetUnscaledDeltaTime() const { return m_deltaTime; }
	float GetTotalTime() const { return m_totalTime; }
	float GetUnscaledTotalTime() const { return m_unscaledTotalTime; }

	void SetTimeScale(float scale);
	float GetTimeScale() const { return m_timeScale; }

	uint32 GetFPS() const { return m_fps; }

	void SetFixedDeltaTime(float fixedDt) { m_fixedDelta = fixedDt; }
	float GetFixedDeltaTime() const { return m_fixedDelta; }

	bool ShouldRunFixedUpdate() const;
	void ConsumeFixedUpdateStep();

private:
	static TimeManager* Instance;

	uint64 m_prevTick{ 0 };

	float m_deltaTime{ 0.0f };
	float m_unscaledTotalTime{ 0.0f };
	float m_totalTime{ 0.0f };

	float m_timeScale{ 1.0f };
	bool m_paused{ false };

	// FPS 계산
	uint64 m_prevFPSCalcTick{ 0 };
	uint32 m_frameCounter{ 0 };
	uint32 m_fps{ 0 };

	// Fixed Update
	float m_fixedDelta{ 1.0f / 60.0f };
	float m_accumulator{ 0.0f };
};