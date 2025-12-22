#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Material/MaterialManager.h"
#include "Engine/Graphics/PSO/PipelineStateLibrary.h"
#include "Engine/Graphics/PSO/BuiltinPipelineIDs.h"
#include "Engine/Core/Utils/StringUtils.h"

// =========================================================
// Destructor
// =========================================================

MaterialManager::~MaterialManager() = default;

// =========================================================
// Material Creation
// =========================================================

Material* MaterialManager::CreateMaterial(const std::wstring& name)
{
    SP_ASSERT(!name.empty());

    auto it = m_materials.find(name);
    if (it != m_materials.end())
        return it->second.get();

    auto material = std::make_unique<Material>(name);
    Material* result = material.get();

    m_materials.emplace(name, std::move(material));

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "Material created: " << StringUtils::ToUTF8(name));
#endif

    return result;
}

Material* MaterialManager::GetMaterial(const std::wstring& name) const
{
    auto it = m_materials.find(name);
    return (it != m_materials.end()) ? it->second.get() : nullptr;
}

bool MaterialManager::HasMaterial(const std::wstring& name) const
{
    return m_materials.find(name) != m_materials.end();
}

// =========================================================
// Built-in Materials
// =========================================================

void MaterialManager::CreateBuiltinMaterials(
    PipelineStateLibrary& pipelineLibrary)
{
    /*
        SpriteAlpha
        -------------------------------------------------
        - 알파 블렌딩 스프라이트
    */
    {
        SP_ASSERT(pipelineLibrary.Has(BuiltinPipelineIDs::SpriteAlpha));

        Material* spriteAlpha = CreateMaterial(L"SpriteAlpha");
        spriteAlpha->SetPipeline(BuiltinPipelineIDs::SpriteAlpha);
    }

    /*
        SpriteOpaque
        -------------------------------------------------
        - 불투명 스프라이트 / UI / Debug
    */
    {
        SP_ASSERT(pipelineLibrary.Has(BuiltinPipelineIDs::SpriteOpaque));

        Material* spriteOpaque = CreateMaterial(L"SpriteOpaque");
        spriteOpaque->SetPipeline(BuiltinPipelineIDs::SpriteOpaque);
    }

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "Built-in Materials created (PipelineID based)");
#endif
}

// =========================================================
// Clear
// =========================================================

void MaterialManager::Clear()
{
    m_materials.clear();
}
