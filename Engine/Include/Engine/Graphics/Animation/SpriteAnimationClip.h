#pragma once

#include <vector>
#include <string>
#include <Windows.h>

struct SpriteAnimationClip
{
	std::wstring name;            // 애니메이션 이름
	std::vector<RECT> frames;     // UV 프레임 목록
	float fps = 12.0f;            // 초당 프레임
	bool loop = true;             // 반복 여부

	bool IsValid() const { return !frames.empty(); }
};