#pragma once

#include <unordered_map>
#include <vector>

#include "Engine/Core/Input/InputAction.h"

/*
    PlayerInput
    -------------------------------------------------
    - 플레이어 단위 입력 상태 컨테이너
    - InputSystem으로부터 InputEvent를 받아 상태 갱신
    - Consume + Input Buffer 지원
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
    void BeginFrame(float deltaTime);

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

public:
    // =====================================================
    // Input Buffer
    // =====================================================
    void BufferAction(InputActionID action, float duration = 0.15f);
    bool ConsumeBuffered(InputActionID action);

private:
    struct ActionState
    {
        bool current{ false };
        bool previous{ false };

        bool consumedPressed{ false };
        bool consumedReleased{ false };
    };

    struct BufferedAction
    {
        InputActionID action;
        float timeLeft;
    };

private:
    ActionState& GetOrCreateState(InputActionID action);
    const ActionState* FindState(InputActionID action) const;

private:
    std::unordered_map<InputActionID, ActionState> m_states;
    std::vector<BufferedAction> m_bufferedActions;
};
