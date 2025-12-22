#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Common/Types.h"
#include "Engine/Graphics/Shader/ShaderID.h"

// Forward declarations
class RenderDevice;
class VertexShader;
class PixelShader;

/*
    ShaderManager
    -------------------------------------------------
    - Shader 로딩 및 수명 관리
    - Name -> ShaderID 매핑
    - ShaderID -> 실제 Shader 객체 Resolve
    - Stage(Vertex / Pixel) 분리로 ID 충돌 방지
*/
class ShaderManager
{
public:
    explicit ShaderManager(RenderDevice* device);
    ~ShaderManager();

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

public:
    // =========================================================
    // Load
    // =========================================================
    ShaderID LoadVertexShader(
        const std::wstring& name,
        const std::wstring& file,
        const std::string& entry);

    ShaderID LoadPixelShader(
        const std::wstring& name,
        const std::wstring& file,
        const std::string& entry);

public:
    // =========================================================
    // Name -> ID
    // =========================================================
    ShaderID GetVertexShaderID(const std::wstring& name) const;
    ShaderID GetPixelShaderID(const std::wstring& name) const;

public:
    // =========================================================
    // Resolve (ID -> Shader)
    // =========================================================
    VertexShader* ResolveVertexShader(ShaderID id) const;
    PixelShader* ResolvePixelShader(ShaderID id) const;

private:
    RenderDevice* m_device{ nullptr };

    // ID generators (stage separated)
    uint32 m_nextVSID{ 1 };
    uint32 m_nextPSID{ 1 };

    // Name -> ID
    std::unordered_map<std::wstring, ShaderID> m_vsNameToID;
    std::unordered_map<std::wstring, ShaderID> m_psNameToID;

    // ID.Value -> Shader object
    std::unordered_map<uint32, std::unique_ptr<VertexShader>> m_vertexShaders;
    std::unordered_map<uint32, std::unique_ptr<PixelShader>>  m_pixelShaders;
};
