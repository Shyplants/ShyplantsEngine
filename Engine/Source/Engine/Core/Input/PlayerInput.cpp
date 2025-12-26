#include "Engine/PCH/EnginePCH.h"
#include "Engine/Core/Input/PlayerInput.h"

// =====================================================
// Frame lifecycle
// =====================================================

void PlayerInput::BeginFrame(float deltaTime)
{
    for (auto& [_, state] : m_states)
    {
        state.previous = state.current;
        state.consumedPressed = false;
        state.consumedReleased = false;
    }

    // Input buffer °¨¼Ò
    for (auto it = m_bufferedActions.begin(); it != m_bufferedActions.end(); )
    {
        it->timeLeft -= deltaTime;
        if (it->timeLeft <= 0.0f)
            it = m_bufferedActions.erase(it);
        else
            ++it;
    }
}

// =====================================================
// Input injection
// =====================================================

void PlayerInput::ApplyAction(InputActionID action, EInputActionState state)
{
    ActionState& s = GetOrCreateState(action);

    switch (state)
    {
    case EInputActionState::Pressed:
        s.current = true;
        BufferAction(action);
        break;

    case EInputActionState::Released:
        s.current = false;
        break;

    case EInputActionState::Held:
        break;

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
    return s && s->current && !s->previous;
}

bool PlayerInput::IsHeld(InputActionID action) const
{
    const ActionState* s = FindState(action);
    return s && s->current;
}

bool PlayerInput::IsReleased(InputActionID action) const
{
    const ActionState* s = FindState(action);
    return s && !s->current && s->previous;
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
// Input Buffer
// =====================================================

void PlayerInput::BufferAction(InputActionID action, float duration)
{
    m_bufferedActions.push_back({ action, duration });
}

bool PlayerInput::ConsumeBuffered(InputActionID action)
{
    for (auto it = m_bufferedActions.begin(); it != m_bufferedActions.end(); ++it)
    {
        if (it->action == action)
        {
            m_bufferedActions.erase(it);
            return true;
        }
    }
    return false;
}

// =====================================================
// Internal
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
    return (it != m_states.end()) ? &it->second : nullptr;
}
