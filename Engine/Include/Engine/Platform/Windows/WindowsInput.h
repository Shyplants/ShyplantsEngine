#pragma once

#include <Windows.h>
#include "Engine/Platform/Input/InputCodes.h"

/*
    WindowsInput
    -------------------------------------------------
    - Win32 메시지를 InputSystem으로 변환
*/
class WindowsInput
{
public:
    static void HandleMessage(
        UINT msg,
        WPARAM wParam,
        LPARAM lParam);

private:
    static KeyCode TranslateKey(WPARAM wParam);
};
