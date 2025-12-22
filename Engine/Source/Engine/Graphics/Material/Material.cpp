#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Material/Material.h"

// =========================================================
// Constructor
// =========================================================

Material::Material(std::wstring name)
    : m_name(std::move(name))
{
    SP_ASSERT(!m_name.empty());
}

Material::~Material() = default;

// =========================================================
// Pipeline Policy
// =========================================================

void Material::SetPipeline(PipelineID pipelineID)
{
    SP_ASSERT(pipelineID != InvalidPipelineID);

#if defined(_DEBUG)
    // Pipeline은 "초기화 단계 1회 설정"이 원칙
    SP_ASSERT(m_pipeline == InvalidPipelineID);
#endif

    m_pipeline = pipelineID;
}

PipelineID Material::GetPipeline() const
{
    SP_ASSERT(m_pipeline != InvalidPipelineID);
    return m_pipeline;
}
