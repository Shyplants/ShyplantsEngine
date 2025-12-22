#pragma once

#include <vector>
#include <string>

#include "Common/Math/Rect.h"

struct SpriteAnimationClip
{
	std::wstring name;            // 애니메이션 이름
	float fps = 12.0f;            // 초당 프레임
	bool loop = true;             // 반복 여부
	std::vector<Rect> frames;     // UV 프레임 목록
};