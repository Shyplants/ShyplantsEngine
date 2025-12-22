#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/PSO/GraphicsPSOManager.h"

#include "Engine/Graphics/PSO/PipelineStateLibrary.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"

#include "Engine/Core/Utils/StringUtils.h"

GraphicsPSOManager::GraphicsPSOManager(
    PipelineStateLibrary* library)
    : m_pipelineLibrary(library)
{
    SP_ASSERT(m_pipelineLibrary != nullptr);
}

GraphicsPSOManager::~GraphicsPSOManager() = default;

GraphicsPSO* GraphicsPSOManager::GetOrCreate(PipelineID pipelineID)
{
    // -------------------------------------------------
    // Validation
    // -------------------------------------------------
    SP_ASSERT(pipelineID != InvalidPipelineID);

    if (!m_pipelineLibrary->Has(pipelineID))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Invalid PipelineID requested: " << StringUtils::ToString(pipelineID));
        return nullptr;
    }

    // -------------------------------------------------
    // Cache lookup
    // -------------------------------------------------
    auto it = m_cache.find(pipelineID);
    if (it != m_cache.end())
        return it->second.get();

    // -------------------------------------------------
    // Create PSO
    // -------------------------------------------------
    const GraphicsPSODesc& desc =
        m_pipelineLibrary->Get(pipelineID);

    auto pso = std::make_unique<GraphicsPSO>();

    if (!pso->Initialize(desc))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to initialize GraphicsPSO (PipelineID="
            << StringUtils::ToString(pipelineID) << ")");
        return nullptr;
    }

    // -------------------------------------------------
    // Cache & return
    // -------------------------------------------------
    GraphicsPSO* raw = pso.get();
    m_cache.emplace(pipelineID, std::move(pso));

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "GraphicsPSO created (PipelineID=" << StringUtils::ToString(pipelineID) << ")");
#endif

    return raw;
}

void GraphicsPSOManager::Clear()
{
    m_cache.clear();
}
