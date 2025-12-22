#pragma once

#include <wrl/client.h>

#include "Common/Types.h"

// Forward declarations
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

/*
    Mesh
    -------------------------------------------------
    - GPU에 바인딩 가능한 기하 데이터 묶음
    - Vertex / Index Buffer 소유
    - Renderer가 Bind 후 Draw 호출
*/
class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

public:
    // =========================================================
    // Initialize
    // =========================================================
    bool Initialize(
        ID3D11Device* device,
        const void* vertexData,
        uint32 vertexStride,
        uint32 vertexCount,
        const uint16* indexData,
        uint32 indexCount);

public:
    // =========================================================
    // Bind
    // =========================================================
    void Bind(ID3D11DeviceContext* context) const;

public:
    // =========================================================
    // Accessors
    // =========================================================
    uint32 GetIndexCount() const { return m_indexCount; }

private:
    // =========================================================
    // Buffers
    // =========================================================
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

private:
    // =========================================================
    // Layout info
    // =========================================================
    uint32 m_vertexStride{ 0 };
    uint32 m_indexCount{ 0 };
};
