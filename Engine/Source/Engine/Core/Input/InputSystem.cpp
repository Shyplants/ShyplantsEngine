#include "Engine/Core/Input/InputSystem.h"
#include "Engine/Platform/Input/KeyboardDevice.h"

std::unordered_map<InputActionID, InputSystem::ActionState> InputSystem::s_actionStates;
std::vector<InputActionBinding> InputSystem::s_bindings;

void InputSystem::Initialize()
{
    s_actionStates.clear();
    s_bindings.clear();
}

void InputSystem::Shutdown()
{
    s_actionStates.clear();
    s_bindings.clear();
}

void InputSystem::BeginFrame()
{
    // 이전 상태 저장
    for (auto& [id, state] : s_actionStates)
    {
        state.previous = state.current;
        state.current = false;
    }

    // 바인딩된 키 검사
    for (const auto& binding : s_bindings)
    {
        if (IsKeyDown(binding.key))
        {
            s_actionStates[binding.action].current = true;
        }
    }
}

void InputSystem::EndFrame()
{
    // TODO
}

void InputSystem::BindAction(InputActionID action, uint32 key)
{
    for (auto& b : s_bindings)
    {
        if (b.action == action && b.key == key)
            return;
    }

    s_bindings.push_back({ action, key });
    s_actionStates[action];
}

bool InputSystem::IsActionPressed(InputActionID action)
{
    auto& s = s_actionStates[action];
    return s.current && !s.previous;
}

bool InputSystem::IsActionHeld(InputActionID action)
{
    auto& s = s_actionStates[action];
    return s.current;
}

bool InputSystem::IsActionReleased(InputActionID action)
{
    auto& s = s_actionStates[action];
    return !s.current && s.previous;
}

bool InputSystem::IsKeyDown(uint32 key)
{
    return KeyboardDevice::IsKeyDown(key);
}
