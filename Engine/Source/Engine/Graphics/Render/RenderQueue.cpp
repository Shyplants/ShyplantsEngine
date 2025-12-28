#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/RenderQueue.h"

// =====================================================
// Submit
// =====================================================

void RenderQueue::Submit(const DrawCommand& cmd)
{
#if defined(_DEBUG)
    SP_ASSERT(cmd.IsValid());
#endif

    const RenderCategory& category = cmd.Category;

    // -------------------------------------------------
    // Pass routing
    // -------------------------------------------------
    if (category.Space == ERenderSpace::Screen)
    {
        m_screenCommands.push_back(cmd);
    }
    else
    {
        // World space
        if (category.Layer == ERenderLayer::Debug)
        {
            m_debugCommands.push_back(cmd);
        }
        else
        {
            m_worldCommands.push_back(cmd);
        }
    }

    m_dirty = true;
}

// =====================================================
// Accessors
// =====================================================

const std::vector<DrawCommand>& RenderQueue::GetWorldPass()
{
    SortIfNeeded();
    return m_worldCommands;
}

const std::vector<DrawCommand>& RenderQueue::GetScreenPass()
{
    SortIfNeeded();
    return m_screenCommands;
}

const std::vector<DrawCommand>& RenderQueue::GetDebugPass()
{
    SortIfNeeded();
    return m_debugCommands;
}

// =====================================================
// Internal
// =====================================================

void RenderQueue::SortIfNeeded()
{
    if (!m_dirty)
        return;

    auto sorter = [](const DrawCommand& a, const DrawCommand& b)
        {
            return a.SortKey < b.SortKey;
        };

    std::sort(m_worldCommands.begin(), m_worldCommands.end(), sorter);
    std::sort(m_screenCommands.begin(), m_screenCommands.end(), sorter);
    std::sort(m_debugCommands.begin(), m_debugCommands.end(), sorter);

    m_dirty = false;
}

// =====================================================
// Lifecycle
// =====================================================

void RenderQueue::Clear()
{
    m_worldCommands.clear();
    m_screenCommands.clear();
    m_debugCommands.clear();
    m_dirty = false;
}

bool RenderQueue::IsEmpty() const
{
    return m_worldCommands.empty()
        && m_screenCommands.empty()
        && m_debugCommands.empty();
}
