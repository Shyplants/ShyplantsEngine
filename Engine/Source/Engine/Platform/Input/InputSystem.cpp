#include "Engine/PCH/EnginePCH.h"

#include "Engine/Platform/Input/InputSystem.h"

std::array<bool, static_cast<size_t>(KeyCode::Count)> InputSystem::s_current{};
std::array<bool, static_cast<size_t>(KeyCode::Count)> InputSystem::s_previous{};

void InputSystem::Initialize()
{
    s_current.fill(false);
    s_previous.fill(false);
}

void InputSystem::Shutdown()
{
}

void InputSystem::BeginFrame()
{
    s_previous = s_current;
}

void InputSystem::EndFrame()
{
}

bool InputSystem::IsKeyDown(KeyCode key)
{
    return s_current[static_cast<size_t>(key)];
}

bool InputSystem::IsKeyPressed(KeyCode key)
{
    size_t i = static_cast<size_t>(key);
    return s_current[i] && !s_previous[i];
}

bool InputSystem::IsKeyReleased(KeyCode key)
{
    size_t i = static_cast<size_t>(key);
    return !s_current[i] && s_previous[i];
}

bool InputSystem::IsMouseDown(KeyCode button)
{
    return IsKeyDown(button);
}

void InputSystem::SetKeyDown(KeyCode key, bool down)
{
    size_t index = static_cast<size_t>(key);
    if (index >= s_current.size())
        return;

    s_current[index] = down;
}
