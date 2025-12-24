#pragma once

#include <unordered_map>
#include <vector>

#include "Engine/Core/Input/InputAction.h"
#include "Engine/Core/Input/InputActionBinding.h"

class InputSystem
{
public:
    // =========================================
    // Lifecycle
    // =========================================
    static void Initialize();
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();

    // =========================================
    // Action Binding
    // =========================================
    static void BindAction(InputActionID action, uint32 key);

    // =========================================
    // Action Query
    // =========================================
    static bool IsActionPressed(InputActionID action);
    static bool IsActionHeld(InputActionID action);
    static bool IsActionReleased(InputActionID action);

private:
    static bool IsKeyDown(uint32 key);

private:
    struct ActionState
    {
        bool current{ false };
        bool previous{ false };
    };

    static std::unordered_map<InputActionID, ActionState> s_actionStates;
    static std::vector<InputActionBinding> s_bindings;
};
