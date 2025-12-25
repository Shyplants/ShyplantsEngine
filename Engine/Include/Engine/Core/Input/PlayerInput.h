#pragma once

#include <unordered_map>

#include "Engine/Core/Input/InputAction.h"

/*
    PlayerInput
    -------------------------------------------------
    - 플레이어 단위 입력 상태 컨테이너
    - InputSystem으로부터 InputEvent를 받아 상태 갱신
    - consumed 개념 포함 (Pressed / Released)
    - PlayerController가 소유
*/
class PlayerInput
{
public:
    PlayerInput() = default;
    ~PlayerInput() = default;

    PlayerInput(const PlayerInput&) = delete;
    PlayerInput& operator=(const PlayerInput&) = delete;

public:
    // =====================================================
    // Frame lifecycle
    // =====================================================
    void BeginFrame();

public:
    // =====================================================
    // Input injection
    // =====================================================
    void ApplyAction(InputActionID action, EInputActionState state);

public:
    // =====================================================
    // Query
    // =====================================================
    bool IsPressed(InputActionID action) const;
    bool IsHeld(InputActionID action) const;
    bool IsReleased(InputActionID action) const;

public:
    // =====================================================
    // Consume
    // =====================================================
    bool ConsumePressed(InputActionID action);
    bool ConsumeReleased(InputActionID action);

private:
    struct ActionState
    {
        bool current{ false };
        bool previous{ false };

        bool consumedPressed{ false };
        bool consumedReleased{ false };
    };

private:
    ActionState& GetOrCreateState(InputActionID action);
    const ActionState* FindState(InputActionID action) const;

private:
    std::unordered_map<InputActionID, ActionState> m_states;
};
