#pragma once

#include <array>
#include "Engine/Platform/Input/InputCodes.h"

/*
    InputSystem
    -------------------------------------------------
    - 플랫폼 독립 입력 상태 저장소
    - WindowsInput / LinuxInput이 값을 갱신
*/
class InputSystem
{
public:
    static void Initialize();
    static void Shutdown();

    // Per-frame
    static void BeginFrame();
    static void EndFrame();

    // State query
    static bool IsKeyDown(KeyCode key);
    static bool IsKeyPressed(KeyCode key);
    static bool IsKeyReleased(KeyCode key);

    static bool IsMouseDown(KeyCode button);

    // Platform update
    static void SetKeyDown(KeyCode key, bool down);

private:
    static std::array<bool, static_cast<size_t>(KeyCode::Count)> s_current;
    static std::array<bool, static_cast<size_t>(KeyCode::Count)> s_previous;
};
