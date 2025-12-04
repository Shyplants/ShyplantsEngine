#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include "Common/Types.h"

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh();

private:
	virtual bool CreateVertexBuffer(ID3D11Device* device) = 0;
	virtual bool CreateIndexBuffer(ID3D11Device* device) = 0;

public:
	ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer.Get(); }
	ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer.Get(); }

	uint32 GetVertexStride() const { return m_vertexStride; }
	uint32 GetVertexOffset() const { return m_vertexOffset; }
	uint32 GetIndexCount() const { return m_indexCount; }

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer{};
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer{};

	uint32 m_vertexStride = 0;
	uint32 m_vertexOffset = 0;
	uint32 m_indexCount = 0;
};