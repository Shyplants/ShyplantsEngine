#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Mesh/Mesh.h"

using Microsoft::WRL::ComPtr;

// =========================================================
// Initialize
// =========================================================

Mesh::~Mesh() = default;

bool Mesh::Initialize(
    ID3D11Device* device,
    const void* vertexData,
    uint32 vertexStride,
    uint32 vertexCount,
    const uint16* indexData,
    uint32 indexCount)
{
    SP_ASSERT(device != nullptr);
    SP_ASSERT(vertexData != nullptr);
    SP_ASSERT(vertexStride > 0);
    SP_ASSERT(vertexCount > 0);
    SP_ASSERT(indexData != nullptr);
    SP_ASSERT(indexCount > 0);

    m_vertexStride = vertexStride;
    m_indexCount = indexCount;

    // -------------------------------------------------
    // Vertex Buffer
    // -------------------------------------------------
    D3D11_BUFFER_DESC vbDesc{};
    vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.ByteWidth = vertexStride * vertexCount;

    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = vertexData;

    HRESULT hr = device->CreateBuffer(
        &vbDesc,
        &vbData,
        m_vertexBuffer.GetAddressOf());

    if (FAILED(hr))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to create VertexBuffer");
        return false;
    }

    // -------------------------------------------------
    // Index Buffer
    // -------------------------------------------------
    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.ByteWidth = sizeof(uint16) * indexCount;

    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = indexData;

    hr = device->CreateBuffer(
        &ibDesc,
        &ibData,
        m_indexBuffer.GetAddressOf());

    if (FAILED(hr))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to create IndexBuffer");
        return false;
    }

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "Mesh initialized (VB + IB)");
#endif

    return true;
}

// =========================================================
// Bind
// =========================================================

void Mesh::Bind(ID3D11DeviceContext* context) const
{
    SP_ASSERT(context != nullptr);
    SP_ASSERT(m_vertexBuffer != nullptr);
    SP_ASSERT(m_indexBuffer != nullptr);

    const UINT stride = m_vertexStride;
    const UINT offset = 0;

    context->IASetVertexBuffers(
        0,
        1,
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset);

    context->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT,
        0);
}
