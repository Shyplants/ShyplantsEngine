#pragma once

#include "Engine/Graphics/PSO/PipelineID.h"

/*
    BuiltinPipelineIDs
    -------------------------------------------------
    - 엔진 내장 파이프라인 식별자
    - PipelineStateLibrary::Initialize()에서
      반드시 이 ID 순서대로 등록되어야 함
*/
namespace BuiltinPipelineIDs
{
    static constexpr PipelineID SpriteAlpha = 1;
    static constexpr PipelineID SpriteOpaque = 2;
}
