#pragma once

#include <string>
#include "Engine/Graphics/PSO/PipelineID.h"
#include "Engine/Graphics/Render/RenderCategory.h"

class MaterialInstance;
class Mesh;

/*
    DrawCommand
    -------------------------------------------------
    - RenderQueue에 쌓이는 순수 Draw 데이터
    - PSO / Pipeline 해석 책임 없음
*/
struct DrawCommand
{
    // Pipeline
    PipelineID Pipeline{ InvalidPipelineID };

    // Resources
    MaterialInstance* MaterialInstance{ nullptr };
    Mesh* Mesh{ nullptr };

    // Draw arguments
    uint32 IndexCount{ 0 };
    uint32 StartIndex{ 0 };
    int32  BaseVertex{ 0 };

    RenderCategory Category;

    // Render sorting
    uint64 SortKey{ 0 };

    bool IsValid() const
    {
        return Pipeline != InvalidPipelineID &&
            MaterialInstance &&
            Mesh &&
            IndexCount > 0;
    }
};
