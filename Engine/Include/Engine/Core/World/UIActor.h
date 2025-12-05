#pragma once

#include "Engine/Core/World/Actor.h"

// UI 전용 Actor
// - 항상 화면 공간(Screen Space)에 렌더링
// - Level::RenderUI() 패스에서만 렌더링됨
// - World 공간 Transform은 무시하고, 위치는 스크린 픽셀 좌표로 해석

class UIActor :public Actor
{
public:
	UIActor();
	~UIActor() override;

	bool IsUIActor() const override { return true; }

protected:
	void OnSpawned() override;
	void BeginPlay() override;
};