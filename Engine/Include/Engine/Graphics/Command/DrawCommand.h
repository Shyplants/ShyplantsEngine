#pragma once

#include "Common/Types.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/ConstantBuffer/SpriteConstantBuffer.h"

struct DrawCommandSortKey
{
    uint64 PSOHashID = 0;
    int renderLayer = 0;
    int contentLayer = 0;

    // 오름차순 정렬 기준
    bool operator<(const DrawCommandSortKey& rhs) const
    {
        if (renderLayer != rhs.renderLayer)
            return renderLayer < rhs.renderLayer;

        if (contentLayer != rhs.contentLayer)
            return contentLayer < rhs.contentLayer;

        return PSOHashID < rhs.PSOHashID;
    }
};

struct DrawCommand
{
    // 파이프라인 상태 해시
    uint64 PSOHashID = 0;

    // 정점/인덱스 버퍼 바인딩 정보
    ID3D11Buffer* vertexBuffer = nullptr;
    uint32 vertexStride = 0;
    uint32 vertexOffset = 0;

    ID3D11Buffer* indexBuffer = nullptr;
    DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
    uint32 firstIndex = 0;
    uint32 numIndices = 0;
    uint32 baseVertexLocation = 0;

    // Primitive Topology
    D3D11_PRIMITIVE_TOPOLOGY primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    // 정점 셰이더 상수 버퍼 or 기타 셰이더 리소스
    ID3D11Buffer* VSConstantBuffer = nullptr;
    ID3D11ShaderResourceView* PSTextureSRV = nullptr;
    ID3D11SamplerState* PSSampler = nullptr;
    SpriteConstantBuffer spriteConstantBuffer = {};


	void Execute(ID3D11DeviceContext* context, const GraphicsPSO* pso) const
	{
        // 파이프라인 상태 적용
        context->IASetInputLayout(pso->GetInputLayout());
        context->VSSetShader(pso->GetVertexShader(), nullptr, 0);
        context->PSSetShader(pso->GetPixelShader(), nullptr, 0);
        context->RSSetState(pso->GetRasterizerState());
        context->OMSetBlendState(pso->GetBlendState(), nullptr, 0xFFFFFFFF);
        context->OMSetDepthStencilState(pso->GetDepthStencilState(), 0);
        context->IASetPrimitiveTopology(pso->GetPrimitiveType());

        context->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);

        context->IASetIndexBuffer(indexBuffer, indexFormat, 0);

        if (VSConstantBuffer)
        {
            D3D11_MAPPED_SUBRESOURCE mapped = {};
            if (SUCCEEDED(context->Map(VSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
            {
                memcpy(mapped.pData, &spriteConstantBuffer, sizeof(SpriteConstantBuffer));
                context->Unmap(VSConstantBuffer, 0);
            }

            context->VSSetConstantBuffers(0, 1, &VSConstantBuffer);
            // context->PSSetConstantBuffers(0, 1, &VSConstantBuffer);
        }

        if (PSTextureSRV)
            context->PSSetShaderResources(0, 1, &PSTextureSRV);

        if (PSSampler)
            context->PSSetSamplers(0, 1, &PSSampler);


        context->DrawIndexed(numIndices, firstIndex, baseVertexLocation);
	}

    void Clear()
    {
        indexFormat = DXGI_FORMAT_R16_UINT;
        primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }

    DrawCommandSortKey sortKey;
};