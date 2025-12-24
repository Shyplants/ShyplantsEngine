#pragma once

#include <vector>
#include <algorithm>

#include "Engine/Graphics/Render/DrawCommand.h"

/*
    RenderQueue
    -------------------------------------------------
    - DrawCommand 수집 및 정렬
    - SortKey 기준으로 정렬된 명령을 제공
    - GPU / PSO / Material 책임 없음
*/
class RenderQueue
{
public:
    // -------------------------------------------------
    // Submit
    // -------------------------------------------------
    void Submit(const DrawCommand& cmd);

    // -------------------------------------------------
    // Access
    // -------------------------------------------------
    const std::vector<DrawCommand>& GetCommands();

    // -------------------------------------------------
    // Lifecycle
    // -------------------------------------------------
    void Clear();
    bool IsEmpty() const;

private:
    void SortIfNeeded();

private:
    std::vector<DrawCommand> m_commands;
    bool m_dirty{ false };
};
