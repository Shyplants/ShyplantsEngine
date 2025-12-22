#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/CommandBuffer.h"

void CommandBuffer::Add(const GPUCommand& cmd)
{
    SP_ASSERT(cmd.PSO);
    SP_ASSERT(cmd.MaterialInstance);
    SP_ASSERT(cmd.Mesh);
    m_commands.push_back(cmd);
}

const std::vector<GPUCommand>& CommandBuffer::GetCommands() const
{
    return m_commands;
}

void CommandBuffer::Clear()
{
    m_commands.clear();
}

bool CommandBuffer::IsEmpty() const
{
    return m_commands.empty();
}
