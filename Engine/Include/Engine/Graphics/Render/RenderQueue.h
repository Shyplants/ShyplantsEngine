#pragma once

#include <vector>
#include <algorithm>

#include "Engine/Graphics/Render/DrawCommand.h"
#include "Engine/Graphics/Render/RenderCategory.h"

/*
    RenderQueue (Phase 4)
    -------------------------------------------------
    - DrawCommand 수집 및 RenderPass 단위 분리
    - World / Screen / Debug Pass 지원
    - Pass 내부는 SortKey 기준 정렬
    - GPU / Camera / RenderSystem 정책 없음
*/
class RenderQueue
{
public:
    // =====================================================
    // Submit
    // =====================================================
    void Submit(const DrawCommand& cmd);

public:
    // =====================================================
    // Pass accessors
    // =====================================================
    const std::vector<DrawCommand>& GetWorldPass();
    const std::vector<DrawCommand>& GetScreenPass();
    const std::vector<DrawCommand>& GetDebugPass();

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    void Clear();
    bool IsEmpty() const;

private:
    void SortIfNeeded();

private:
    // -------------------------------------------------
    // Pass-separated command lists
    // -------------------------------------------------
    std::vector<DrawCommand> m_worldCommands;
    std::vector<DrawCommand> m_screenCommands;
    std::vector<DrawCommand> m_debugCommands;

    bool m_dirty{ false };
};
