#pragma once

#include <wrl/client.h>
#include <d3dcommon.h>

// Forward declarations
struct ID3D11DeviceContext;

class VertexShader;
class PixelShader;
struct GraphicsPSODesc;

/*
    GraphicsPSO
    -------------------------------------------------
    - DX11 Graphics Pipeline State Object
    - 완전 불변 객체
    - Bind()만 수행 (결정 로직 없음)
*/
class GraphicsPSO
{
public:
    GraphicsPSO() = default;
    ~GraphicsPSO();

    GraphicsPSO(const GraphicsPSO&) = delete;
    GraphicsPSO& operator=(const GraphicsPSO&) = delete;

public:
    // =========================================================
    // Initialize (one-time)
    // =========================================================
    bool Initialize(const GraphicsPSODesc& desc);

public:
    // =========================================================
    // Bind
    // =========================================================
    void Bind(ID3D11DeviceContext* context) const;

private:
    // ---------------------------------------------------------
    // Shaders (non-owning)
    // ---------------------------------------------------------
    VertexShader* m_vs{ nullptr };
    PixelShader* m_ps{ nullptr };

private:
    // ---------------------------------------------------------
    // Fixed states
    // ---------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11BlendState>        m_blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

private:
    // ---------------------------------------------------------
    // IA
    // ---------------------------------------------------------
    D3D11_PRIMITIVE_TOPOLOGY m_topology{
        D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED
    };
};
