#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/Input/PlayerInput.h"

// =====================================================
// Frame lifecycle
// =====================================================

void PlayerInput::BeginFrame()
{
    for (auto& [_, state] : m_states)
    {
        state.previous = state.current;
        state.consumedPressed = false;
        state.consumedReleased = false;
    }
}

// =====================================================
// Input injection
// =====================================================

void PlayerInput::ApplyAction(
    InputActionID action,
    EInputActionState state)
{
    ActionState& s = GetOrCreateState(action);

    switch (state)
    {
    case EInputActionState::Pressed:
        s.current = true;
        break;

    case EInputActionState::Released:
        s.current = false;
        break;

    case EInputActionState::Held:
        // Held는 상태 변화 없음 (current 유지)
        break;

    case EInputActionState::None:
    default:
        break;
    }
}

// =====================================================
// Query
// =====================================================

bool PlayerInput::IsPressed(InputActionID action) const
{
    const ActionState* s = FindState(action);
    if (!s)
        return false;

    return s->current && !s->previous;
}

bool PlayerInput::IsHeld(InputActionID action) const
{
    const ActionState* s = FindState(action);
    if (!s)
        return false;

    return s->current;
}

bool PlayerInput::IsReleased(InputActionID action) const
{
    const ActionState* s = FindState(action);
    if (!s)
        return false;

    return !s->current && s->previous;
}

// =====================================================
// Consume
// =====================================================

bool PlayerInput::ConsumePressed(InputActionID action)
{
    ActionState& s = GetOrCreateState(action);

    if (s.current && !s.previous && !s.consumedPressed)
    {
        s.consumedPressed = true;
        return true;
    }

    return false;
}

bool PlayerInput::ConsumeReleased(InputActionID action)
{
    ActionState& s = GetOrCreateState(action);

    if (!s.current && s.previous && !s.consumedReleased)
    {
        s.consumedReleased = true;
        return true;
    }

    return false;
}

// =====================================================
// Internal helpers
// =====================================================

PlayerInput::ActionState&
PlayerInput::GetOrCreateState(InputActionID action)
{
    return m_states[action];
}

const PlayerInput::ActionState*
PlayerInput::FindState(InputActionID action) const
{
    auto it = m_states.find(action);
    if (it == m_states.end())
        return nullptr;

    return &it->second;
}
