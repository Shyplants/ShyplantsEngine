#pragma once

// Forward declarations only
class Material;
class Mesh;

struct RenderDefaults
{
    Material* SpriteMaterial{ nullptr };
    Mesh* SpriteMesh{ nullptr };
    Mesh* UISpriteMesh{ nullptr };
};
