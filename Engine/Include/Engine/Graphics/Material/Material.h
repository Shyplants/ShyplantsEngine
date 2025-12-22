#pragma once

#include <string>

#include "Engine/Graphics/PSO/PipelineID.h"

/*
    Material
    -------------------------------------------------
    - 렌더링 "정책 객체"
    - PipelineID만 소유 (문자열 제거)
    - GPU 리소스 / PSO / DX 객체 소유 X
*/
class Material
{
public:
    explicit Material(std::wstring name);
    ~Material();

    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

public:
    // =========================================================
    // Identity
    // =========================================================
    const std::wstring& GetName() const { return m_name; }

public:
    // =========================================================
    // Pipeline Policy (Single Source of Truth)
    // =========================================================
    void SetPipeline(PipelineID pipelineID);
    PipelineID GetPipeline() const;

private:
    std::wstring m_name;
    PipelineID   m_pipeline{ InvalidPipelineID };
};
