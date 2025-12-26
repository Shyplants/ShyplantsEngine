#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Input/InputSystem.h"
#include "Engine/Core/Input/PlayerInput.h"
#include "Engine/Platform/Input/KeyboardDevice.h"

std::unordered_map<uint32, InputSystem::KeyState> InputSystem::s_keyStates;
std::vector<InputActionBinding> InputSystem::s_bindings;

// =====================================================
// Lifecycle
// =====================================================

void InputSystem::Initialize()
{
    s_keyStates.clear();
    s_bindings.clear();
}

void InputSystem::Shutdown()
{
    s_keyStates.clear();
    s_bindings.clear();
}

void InputSystem::BeginFrame()
{
    // 이전 프레임 상태 보존
    for (auto& [_, state] : s_keyStates)
    {
        state.previous = state.current;
    }

    // 현재 키 상태 갱신
    for (auto& binding : s_bindings)
    {
        s_keyStates[binding.key].current = IsKeyDown(binding.key);
    }
}

void InputSystem::EndFrame()
{
    // 현재는 처리 없음
}

// =====================================================
// Action Binding
// =====================================================

void InputSystem::BindAction(InputActionID action, uint32 key)
{
    for (const auto& b : s_bindings)
    {
        if (b.action == action && b.key == key)
            return;
    }

    s_bindings.push_back({ action, key });
    s_keyStates[key]; // ensure exists
}

// =====================================================
// Dispatch
// =====================================================

void InputSystem::Dispatch(PlayerInput& playerInput)
{
    for (const auto& binding : s_bindings)
    {
        auto it = s_keyStates.find(binding.key);
        if (it == s_keyStates.end())
            continue;

        const KeyState& ks = it->second;

        if (ks.current && !ks.previous)
        {
            playerInput.ApplyAction(
                binding.action,
                EInputActionState::Pressed);
        }
        else if (ks.current && ks.previous)
        {
            playerInput.ApplyAction(
                binding.action,
                EInputActionState::Held);
        }
        else if (!ks.current && ks.previous)
        {
            playerInput.ApplyAction(
                binding.action,
                EInputActionState::Released);
        }
    }
}

// =====================================================
// Internal
// =====================================================

bool InputSystem::IsKeyDown(uint32 key)
{
    return KeyboardDevice::IsKeyDown(key);
}
