#pragma once

#include <wrl/client.h>
#include <d3d11.h>

// Forward declarations
class VertexShader;
class PixelShader;

/*
    GraphicsPSODesc
    -------------------------------------------------
    - GraphicsPSO 생성에 필요한 "완전히 해석된" 상태 묶음
    - ShaderManager / PipelineStateLibrary 단계에서 해석 완료
    - GraphicsPSO는 이 Desc를 그대로 복사만 함
*/
struct GraphicsPSODesc
{
    // -------------------------------------------------
    // Shaders (resolved, non-owning)
    // -------------------------------------------------
    VertexShader* VS{ nullptr };
    PixelShader* PS{ nullptr };

    // -------------------------------------------------
    // Fixed pipeline states (immutable)
    // -------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       InputLayout;
    Microsoft::WRL::ComPtr<ID3D11BlendState>        BlendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   RasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;

    // -------------------------------------------------
    // IA
    // -------------------------------------------------
    D3D11_PRIMITIVE_TOPOLOGY Topology{
        D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED
    };
};
