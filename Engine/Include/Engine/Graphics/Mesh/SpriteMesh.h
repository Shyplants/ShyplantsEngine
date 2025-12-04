#pragma once

#include "Mesh.h"
#include "Engine/Graphics/Vertex/SpriteVertex.h"

class SpriteMesh : public Mesh
{
public:
	SpriteMesh();
	~SpriteMesh() override;

private:
	bool CreateVertexBuffer(ID3D11Device* device) override;
	bool CreateIndexBuffer(ID3D11Device* device) override;

public:
	bool Init(ID3D11Device* device);
};