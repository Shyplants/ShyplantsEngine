#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Common/Types.h"
#include "Engine/Graphics/Mesh/Mesh.h"

// Forward declarations

struct ID3D11Device;

/*
    MeshManager
    -------------------------------------------------
    - Mesh 府家胶 包府磊
    - 吝汗 积己 规瘤
    - Built-in Mesh 力傍
*/
class MeshManager
{
public:
    explicit MeshManager(ID3D11Device* device);
    ~MeshManager();

    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

public:
    // =========================================================
    // Create
    // =========================================================
    Mesh* CreateMesh(
        const std::wstring& name,
        const void* vertexData,
        uint32 vertexStride,
        uint32 vertexCount,
        const uint16* indexData,
        uint32 indexCount);

public:
    // =========================================================
    // Get
    // =========================================================
    Mesh* GetMesh(const std::wstring& name) const;

public:
    // =========================================================
    // Built-in
    // =========================================================
    void CreateBuiltinMeshes();
    Mesh* GetQuad() const;
    Mesh* GetUIQuad() const;

public:
    // =========================================================
    // Maintenance
    // =========================================================
    void Clear();

private:
    ID3D11Device* m_device{ nullptr };

    std::unordered_map<std::wstring, std::unique_ptr<Mesh>> m_meshes;

    // Built-in
    Mesh* m_quad{ nullptr };
    Mesh* m_uiQuad{ nullptr };
};
