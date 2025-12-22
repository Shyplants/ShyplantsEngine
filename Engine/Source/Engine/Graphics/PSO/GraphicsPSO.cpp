#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/PSO/GraphicsPSODesc.h"

#include "Engine/Graphics/Shader/VertexShader.h"
#include "Engine/Graphics/Shader/PixelShader.h"

using Microsoft::WRL::ComPtr;

// =========================================================
// Destructor
// =========================================================

GraphicsPSO::~GraphicsPSO() = default;

// =========================================================
// Initialize
// =========================================================

bool GraphicsPSO::Initialize(const GraphicsPSODesc& desc)
{
    // -------------------------------------------------
    // Validation (fail fast)
    // -------------------------------------------------
    SP_ASSERT(m_vs == nullptr && "GraphicsPSO already initialized");

    SP_ASSERT(desc.VS != nullptr);
    SP_ASSERT(desc.PS != nullptr);
    SP_ASSERT(desc.InputLayout != nullptr);
    SP_ASSERT(desc.Topology != D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

    // -------------------------------------------------
    // Shaders
    // -------------------------------------------------
    m_vs = desc.VS;
    m_ps = desc.PS;

    // -------------------------------------------------
    // Fixed pipeline states
    // -------------------------------------------------
    m_inputLayout = desc.InputLayout;
    m_blendState = desc.BlendState;
    m_rasterizerState = desc.RasterizerState;
    m_depthStencilState = desc.DepthStencilState;

    // -------------------------------------------------
    // IA
    // -------------------------------------------------
    m_topology = desc.Topology;

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "GraphicsPSO initialized");
#endif

    return true;
}

// =========================================================
// Bind
// =========================================================

void GraphicsPSO::Bind(ID3D11DeviceContext* context) const
{
    SP_ASSERT(context);
    SP_ASSERT(m_vs);
    SP_ASSERT(m_ps);

    // -------------------------------------------------
    // IA
    // -------------------------------------------------
    context->IASetInputLayout(m_inputLayout.Get());
    context->IASetPrimitiveTopology(m_topology);

    // -------------------------------------------------
    // Shaders
    // -------------------------------------------------
    context->VSSetShader(m_vs->GetShader(), nullptr, 0);
    context->PSSetShader(m_ps->GetShader(), nullptr, 0);

    // -------------------------------------------------
    // OM / RS States
    // -------------------------------------------------
    if (m_blendState)
    {
        constexpr float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
        context->OMSetBlendState(
            m_blendState.Get(),
            blendFactor,
            0xFFFFFFFF);
    }

    if (m_depthStencilState)
    {
        context->OMSetDepthStencilState(
            m_depthStencilState.Get(),
            0);
    }

    if (m_rasterizerState)
    {
        context->RSSetState(
            m_rasterizerState.Get());
    }
}
