#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Engine/Graphics/Material/Material.h"
#include "Engine/Graphics/PSO/PipelineID.h"

// Forward declarations
class PipelineStateLibrary;

/*
    MaterialManager
    -------------------------------------------------
    - Material 생성 / 소유 / 조회
    - Material은 PipelineID만 가진다
    - Pipeline 정책은 PipelineStateLibrary에서 등록
*/
class MaterialManager
{
public:
    MaterialManager() = default;
    ~MaterialManager();

    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager&) = delete;

public:
    // =========================================================
    // Access
    // =========================================================
    Material* CreateMaterial(const std::wstring& name);
    Material* GetMaterial(const std::wstring& name) const;
    bool HasMaterial(const std::wstring& name) const;

public:
    // =========================================================
    // Built-in
    // =========================================================
    void CreateBuiltinMaterials(
        PipelineStateLibrary& pipelineLibrary);

    void Clear();

private:
    std::unordered_map<
        std::wstring,
        std::unique_ptr<Material>
    > m_materials;
};
