#include "Engine/Core/EnginePCH.h"
#include "Engine/Graphics/Mesh/SpriteMesh.h"

using namespace DirectX;

SpriteMesh::SpriteMesh()
{
    
}

SpriteMesh::~SpriteMesh()
{
}

bool SpriteMesh::CreateVertexBuffer(ID3D11Device* device)
{
    SpriteVertex vertices[4];

    // local quad from -0.5 ~ +0.5
    vertices[0] = { XMFLOAT3(-0.5f, +0.5f, 0.0f), XMFLOAT4(1,1,1,1), XMFLOAT2(0.0f, 0.0f) }; // top-left
    vertices[1] = { XMFLOAT3(+0.5f, +0.5f, 0.0f), XMFLOAT4(1,1,1,1), XMFLOAT2(1.0f, 0.0f) }; // top-right
    vertices[2] = { XMFLOAT3(+0.5f, -0.5f, 0.0f), XMFLOAT4(1,1,1,1), XMFLOAT2(1.0f, 1.0f) }; // bottom-right
    vertices[3] = { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1,1,1,1), XMFLOAT2(0.0f, 1.0f) }; // bottom-left

    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vbDesc.ByteWidth = sizeof(vertices);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.StructureByteStride = sizeof(SpriteVertex);

    m_vertexStride = sizeof(SpriteVertex);
    m_vertexOffset = 0;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices;

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, m_vertexBuffer.GetAddressOf())))
    {
        __debugbreak();
        return false;
    }

    return true;
}

bool SpriteMesh::CreateIndexBuffer(ID3D11Device* device)
{
    // ÀÎµ¦½º
    uint16 indices[6] = 
    { 
        0, 1, 2, 
        0, 2, 3 
    };
    m_indexCount = 6;

    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices;

    if (FAILED(device->CreateBuffer(&ibDesc, &ibData, m_indexBuffer.GetAddressOf())))
    {
        __debugbreak();
        return false;
    }

    return true;
}

bool SpriteMesh::Init(ID3D11Device* device)
{
    if (!CreateVertexBuffer(device))
        return false;

    if (!CreateIndexBuffer(device))
        return false;

    return true;
}
