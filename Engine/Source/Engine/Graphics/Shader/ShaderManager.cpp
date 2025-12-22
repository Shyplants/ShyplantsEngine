#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Shader/ShaderManager.h"
#include "Engine/Graphics/Shader/VertexShader.h"
#include "Engine/Graphics/Shader/PixelShader.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"

#include "Engine/Core/Utils/StringUtils.h"

// =========================================================
// Constructor / Destructor
// =========================================================

ShaderManager::ShaderManager(RenderDevice* device)
    : m_device(device)
{
    SP_ASSERT(m_device != nullptr);
}

ShaderManager::~ShaderManager() = default;

// =========================================================
// Load
// =========================================================

ShaderID ShaderManager::LoadVertexShader(
    const std::wstring& name,
    const std::wstring& file,
    const std::string& entry)
{
    SP_ASSERT(!name.empty());

    if (auto it = m_vsNameToID.find(name); it != m_vsNameToID.end())
        return it->second;

    auto shader = std::make_unique<VertexShader>(m_device);

    if (!shader->Load(file, entry))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to load VertexShader: " << StringUtils::ToUTF8(name));
        return InvalidVertexShaderID; // Fatal 이므로 도달하지 않는 경로
    }

    ShaderID id{ m_nextVSID++, ShaderStage::Vertex };

    m_vsNameToID.emplace(name, id);
    m_vertexShaders.emplace(id.Value, std::move(shader));

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "VertexShader loaded: " << StringUtils::ToUTF8(name)
        << " (ID=" << id.Value << ")");
#endif

    return id;
}

ShaderID ShaderManager::LoadPixelShader(
    const std::wstring& name,
    const std::wstring& file,
    const std::string& entry)
{
    SP_ASSERT(!name.empty());

    if (auto it = m_psNameToID.find(name); it != m_psNameToID.end())
        return it->second;

    auto shader = std::make_unique<PixelShader>(m_device);

    if (!shader->Load(file, entry))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to load PixelShader: " << StringUtils::ToUTF8(name));
        return InvalidPixelShaderID;
    }

    ShaderID id{ m_nextPSID++, ShaderStage::Pixel };

    m_psNameToID.emplace(name, id);
    m_pixelShaders.emplace(id.Value, std::move(shader));

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "PixelShader loaded: " << StringUtils::ToUTF8(name)
        << " (ID=" << id.Value << ")");
#endif

    return id;
}

// =========================================================
// Name -> ID
// =========================================================

ShaderID ShaderManager::GetVertexShaderID(
    const std::wstring& name) const
{
    auto it = m_vsNameToID.find(name);
    SP_ASSERT(it != m_vsNameToID.end());
    return it->second;
}

ShaderID ShaderManager::GetPixelShaderID(
    const std::wstring& name) const
{
    auto it = m_psNameToID.find(name);
    SP_ASSERT(it != m_psNameToID.end());
    return it->second;
}

// =========================================================
// Resolve
// =========================================================

VertexShader* ShaderManager::ResolveVertexShader(
    ShaderID id) const
{
    SP_ASSERT(id.Stage == ShaderStage::Vertex);

    auto it = m_vertexShaders.find(id.Value);
    SP_ASSERT(it != m_vertexShaders.end());
    return it->second.get();
}

PixelShader* ShaderManager::ResolvePixelShader(
    ShaderID id) const
{
    SP_ASSERT(id.Stage == ShaderStage::Pixel);

    auto it = m_pixelShaders.find(id.Value);
    SP_ASSERT(it != m_pixelShaders.end());
    return it->second.get();
}
