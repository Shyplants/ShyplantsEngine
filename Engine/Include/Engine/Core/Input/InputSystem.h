#pragma once

#include <unordered_map>
#include <vector>

#include "Engine/Core/Input/InputAction.h"
#include "Engine/Core/Input/InputActionBinding.h"

// Forward declarations
class PlayerInput;

/*
    InputSystem
    -------------------------------------------------
    - 전역 Raw Input → InputAction 변환 시스템
    - 상태를 저장하지 않음
    - PlayerInput으로 이벤트를 전달
*/
class InputSystem
{
public:
    // =====================================================
    // Lifecycle
    // =====================================================
    static void Initialize();
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();

public:
    // =====================================================
    // Action Binding
    // =====================================================
    static void BindAction(InputActionID action, uint32 key);

public:
    // =====================================================
    // Input Dispatch
    // =====================================================
    static void Dispatch(PlayerInput& playerInput);

private:
    static bool IsKeyDown(uint32 key);

private:
    struct KeyState
    {
        bool current{ false };
        bool previous{ false };
    };

private:
    static std::unordered_map<uint32, KeyState> s_keyStates;
    static std::vector<InputActionBinding> s_bindings;
};
