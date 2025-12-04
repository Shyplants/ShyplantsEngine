#pragma once

#include <windows.h>
#include <array>
#include "Common/Types.h"

enum class KeyCode : uint8
{
    Left = VK_LEFT,
    Right = VK_RIGHT,
    Up = VK_UP,
    Down = VK_DOWN,

    Space = VK_SPACE,
    Enter = VK_RETURN,
    Escape = VK_ESCAPE,

    A = 'A',
    D = 'D',
    S = 'S',
    W = 'W',
};

class InputManager
{
public:
    static void Create();
    static void Destroy();
    static bool IsValid();
    static InputManager& Get();

private:
    InputManager() = default;
    ~InputManager() = default;

public:
    // WindowProc에서 호출
    void ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    void Update();

    // Key states
    bool IsKeyDown(KeyCode key) const;        // 현재 눌린 상태
    bool IsKeyPressed(KeyCode key) const;     // 이번 프레임에서 눌림
    bool IsKeyReleased(KeyCode key) const;    // 이번 프레임에서 떼어짐

private:
    static InputManager* Instance;

    std::array<bool, 256> m_curKeys{};   // 현재 프레임 키 상태
    std::array<bool, 256> m_prevKeys{};  // 이전 프레임 키 상태
};