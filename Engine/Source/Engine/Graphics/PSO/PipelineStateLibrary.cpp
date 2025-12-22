#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/PSO/PipelineStateLibrary.h"
#include "Engine/Graphics/PSO/BuiltinPipelineIDs.h"

#include "Engine/Graphics/Shader/ShaderManager.h"
#include "Engine/Graphics/Shader/VertexShader.h"
#include "Engine/Graphics/Shader/PixelShader.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"


// =========================================================
// Initialize
// =========================================================

PipelineStateLibrary::~PipelineStateLibrary() = default;

void PipelineStateLibrary::Initialize(
    ShaderManager& shaderManager,
    RenderDevice& renderDevice)
{
    m_pipelines.clear();

#ifdef _DEBUG
    m_debugNames.clear();
#endif

    // Reserve index 0 as InvalidPipelineID
    m_pipelines.emplace_back();

    RegisterBuiltinPipelines(shaderManager, renderDevice);
}


PipelineID PipelineStateLibrary::Register(
    const std::wstring& pipelineName,
    const GraphicsPSODesc& desc)
{
    SP_ASSERT(!m_pipelines.empty());

    PipelineID id = static_cast<PipelineID>(m_pipelines.size());
    m_pipelines.push_back(desc);

#ifdef _DEBUG
    m_debugNames.emplace(id, pipelineName);
#endif

    return id;
}

// =========================================================
// Access
// =========================================================

const GraphicsPSODesc& PipelineStateLibrary::Get(PipelineID id) const
{
    SP_ASSERT(id != InvalidPipelineID);
    SP_ASSERT(id < m_pipelines.size());

    return m_pipelines[id];
}

bool PipelineStateLibrary::Has(PipelineID id) const
{
    return id != InvalidPipelineID &&
        id < m_pipelines.size();
}

#ifdef _DEBUG
const std::wstring& PipelineStateLibrary::GetDebugName(PipelineID id) const
{
    static const std::wstring kInvalid = L"<InvalidPipeline>";

    auto it = m_debugNames.find(id);
    if (it == m_debugNames.end())
        return kInvalid;

    return it->second;
}
#endif

// =========================================================
// Built-in Pipelines
// =========================================================

void PipelineStateLibrary::RegisterBuiltinPipelines(
    ShaderManager& shaderManager,
    RenderDevice& renderDevice)
{
    /*
        SpriteAlpha
        -------------------------------------------------
        - 2D Sprite
        - Alpha Blending
        - Depth Read Only
    */
    {
        GraphicsPSODesc desc{};

        // ---------------------------------------------
        // Resolve shaders
        // ---------------------------------------------
        const ShaderID vsID = shaderManager.GetVertexShaderID(L"SpriteVS");
        const ShaderID psID = shaderManager.GetPixelShaderID(L"SpritePS");
            

        VertexShader* vs = shaderManager.ResolveVertexShader(vsID);
        PixelShader* ps = shaderManager.ResolvePixelShader(psID);
            

        desc.VS = vs;
        desc.PS = ps;

        // ---------------------------------------------
        // Fixed pipeline states
        // ---------------------------------------------
        desc.InputLayout = renderDevice.CreateSpriteInputLayout(vs);
            
        desc.BlendState = renderDevice.CreateAlphaBlendState();
            
        desc.RasterizerState = renderDevice.CreateCullNoneRasterizer();
            
        desc.DepthStencilState = renderDevice.CreateDepthReadState();
            
        desc.Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            

        PipelineID id = Register(L"SpriteAlpha", desc);
        SP_ASSERT(id == BuiltinPipelineIDs::SpriteAlpha);
    }

    /*
        SpriteOpaque
        -------------------------------------------------
        - Alpha พ๘ดย Sprite
        - UI / Debug Rendering
    */
    {
        GraphicsPSODesc desc{};

        const ShaderID vsID = shaderManager.GetVertexShaderID(L"SpriteVS");    
        const ShaderID psID = shaderManager.GetPixelShaderID(L"SpritePS");
            
        VertexShader* vs = shaderManager.ResolveVertexShader(vsID); 
        PixelShader* ps = shaderManager.ResolvePixelShader(psID);
            

        desc.VS = vs;
        desc.PS = ps;

        desc.InputLayout = renderDevice.CreateSpriteInputLayout(vs);
            

        desc.BlendState = nullptr;

        desc.RasterizerState = renderDevice.CreateCullNoneRasterizer();
            
        desc.DepthStencilState = renderDevice.CreateDepthReadState();
            
        desc.Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            

        PipelineID id = Register(L"SpriteOpaque", desc);
        SP_ASSERT(id == BuiltinPipelineIDs::SpriteOpaque);
    }
}
