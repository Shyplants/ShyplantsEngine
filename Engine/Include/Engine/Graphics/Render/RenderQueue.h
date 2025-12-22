#pragma once

#include <vector>
#include "Engine/Graphics/Render/DrawCommand.h"

/*
    RenderQueue
    -------------------------------------------------
    - DrawCommand 수집 전용
    - Pipeline / PSO / Shader 관여 금지
*/
class RenderQueue
{
public:
    void Submit(const DrawCommand& cmd);
    const std::vector<DrawCommand>& GetCommands() const;

    void Clear();
    bool IsEmpty() const;

private:
    std::vector<DrawCommand> m_commands;
};
