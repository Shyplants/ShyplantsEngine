#pragma once

#include "Common/Types.h"

/*
    Level Type
    -------------------------------------------------
    - World가 Level의 역할을 구분하기 위한 메타 정보
*/
enum class ELevelType : uint8
{
    Persistent,
    Gameplay
};
