#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/RenderQueue.h"

void RenderQueue::Submit(const DrawCommand& cmd)
{
    SP_ASSERT(cmd.IsValid());
    m_commands.push_back(cmd);
}

const std::vector<DrawCommand>& RenderQueue::GetCommands() const
{
    return m_commands;
}

void RenderQueue::Clear()
{
    m_commands.clear();
}

bool RenderQueue::IsEmpty() const
{
    return m_commands.empty();
}
