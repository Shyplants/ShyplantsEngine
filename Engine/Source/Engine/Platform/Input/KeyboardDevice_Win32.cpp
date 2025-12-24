#include "Engine/Platform/Input/KeyboardDevice.h"

bool KeyboardDevice::s_keys[256] = {};

void KeyboardDevice::SetKeyDown(uint32 key, bool down)
{
    if (key < 256)
        s_keys[key] = down;
}

bool KeyboardDevice::IsKeyDown(uint32 key)
{
    return key < 256 ? s_keys[key] : false;
}
