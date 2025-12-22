#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Engine/Graphics/PSO/GraphicsPSODesc.h"
#include "Engine/Graphics/PSO/PipelineID.h"

// Forward declarations
class ShaderManager;
class RenderDevice;

/*
    PipelineStateLibrary
    -------------------------------------------------
    - 엔진이 허용하는 Graphics Pipeline "정책 집합"
    - PipelineID → GraphicsPSODesc 매핑
    - 문자열 이름은 등록 단계에서만 사용
    - PSO 생성 / 캐시 책임 없음
*/
class PipelineStateLibrary
{
public:
    PipelineStateLibrary() = default;
    ~PipelineStateLibrary();

    PipelineStateLibrary(const PipelineStateLibrary&) = delete;
    PipelineStateLibrary& operator=(const PipelineStateLibrary&) = delete;

public:
    // =========================================================
    // Initialize (engine startup, one-time)
    // =========================================================
    void Initialize(
        ShaderManager& shaderManager,
        RenderDevice& renderDevice);

public:
    // =========================================================
    // Registration
    // =========================================================
    PipelineID Register(
        const std::wstring& pipelineName,
        const GraphicsPSODesc& desc);

public:
    // =========================================================
    // Access
    // =========================================================
    const GraphicsPSODesc& Get(PipelineID id) const;
    bool Has(PipelineID id) const;

#ifdef _DEBUG
public:
    const std::wstring& GetDebugName(PipelineID id) const;
#endif

private:
    // =========================================================
    // Built-in registration
    // =========================================================
    void RegisterBuiltinPipelines(
        ShaderManager& shaderManager,
        RenderDevice& renderDevice);

private:
    // =========================================================
    // Storage
    // =========================================================
    std::vector<GraphicsPSODesc> m_pipelines;

#ifdef _DEBUG
    std::unordered_map<PipelineID, std::wstring> m_debugNames;
#endif
};
