#pragma once
#include "Engine/Core/Input/InputAction.h"

struct InputActionBinding
{
    InputActionID action;
    uint32        key;     // VK_LEFT, 'A', etc (Win32)
};