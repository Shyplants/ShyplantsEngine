#pragma once
#include "Common/Types.h"

using InputActionID = uint16;

enum class EInputActionState : uint8
{
    None,
    Pressed,   // 이번 프레임에 눌림
    Held,      // 계속 누르고 있음
    Released   // 이번 프레임에 뗌
};