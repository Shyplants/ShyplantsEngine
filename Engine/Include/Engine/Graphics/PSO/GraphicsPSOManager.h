#pragma once

#include <unordered_map>
#include <memory>

#include "Engine/Graphics/PSO/PipelineID.h"

// Forward declarations
class GraphicsPSO;
class PipelineStateLibrary;

/*
    GraphicsPSOManager
    -------------------------------------------------
    - PipelineID → GraphicsPSO 캐시
    - PSO 생성 책임 보유
    - Pipeline 정책은 PipelineStateLibrary에 위임
*/
class GraphicsPSOManager
{
public:
    explicit GraphicsPSOManager(PipelineStateLibrary* library);
    ~GraphicsPSOManager();

    GraphicsPSOManager(const GraphicsPSOManager&) = delete;
    GraphicsPSOManager& operator=(const GraphicsPSOManager&) = delete;

public:
    GraphicsPSO* GetOrCreate(PipelineID pipelineID);
    void Clear();

private:
    PipelineStateLibrary* m_pipelineLibrary{ nullptr };

    std::unordered_map<
        PipelineID,
        std::unique_ptr<GraphicsPSO>
    > m_cache;
};
