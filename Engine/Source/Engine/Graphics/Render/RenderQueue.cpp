#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/RenderQueue.h"

// =====================================================
// Submit
// =====================================================

void RenderQueue::Submit(const DrawCommand& cmd)
{
    SP_ASSERT(cmd.IsValid());

    m_commands.push_back(cmd);
    m_dirty = true;
}

// =====================================================
// Access
// =====================================================

const std::vector<DrawCommand>& RenderQueue::GetCommands()
{
    SortIfNeeded();
    return m_commands;
}

// =====================================================
// Internal
// =====================================================

void RenderQueue::SortIfNeeded()
{
    if (!m_dirty)
        return;

    std::sort(
        m_commands.begin(),
        m_commands.end(),
        [](const DrawCommand& a, const DrawCommand& b)
        {
            return a.SortKey < b.SortKey;
        });

    m_dirty = false;
}

// =====================================================
// Lifecycle
// =====================================================

void RenderQueue::Clear()
{
    m_commands.clear();
    m_dirty = false;
}

bool RenderQueue::IsEmpty() const
{
    return m_commands.empty();
}
