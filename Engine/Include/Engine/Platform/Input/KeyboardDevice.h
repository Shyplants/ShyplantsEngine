#pragma once
#include "Common/Types.h"

class KeyboardDevice
{
public:
    static void SetKeyDown(uint32 key, bool down);
    static bool IsKeyDown(uint32 key);

private:
    static bool s_keys[256];
};
