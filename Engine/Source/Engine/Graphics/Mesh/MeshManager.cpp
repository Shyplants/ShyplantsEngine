#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Mesh/MeshManager.h"

#include "Engine/Core/Utils/StringUtils.h"

// =========================================================
// Constructor
// =========================================================

MeshManager::MeshManager(ID3D11Device* device)
    : m_device(device)
{
    SP_ASSERT(m_device != nullptr);
}

MeshManager::~MeshManager() = default;

// =========================================================
// Create Mesh
// =========================================================

Mesh* MeshManager::CreateMesh(
    const std::wstring& name,
    const void* vertexData,
    uint32 vertexStride,
    uint32 vertexCount,
    const uint16* indexData,
    uint32 indexCount)
{
    auto it = m_meshes.find(name);
    if (it != m_meshes.end())
        return it->second.get();

    auto mesh = std::make_unique<Mesh>();
    if (!mesh->Initialize(
        m_device,
        vertexData,
        vertexStride,
        vertexCount,
        indexData,
        indexCount))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to create Mesh");
        return nullptr;
    }

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "Mesh created: " << StringUtils::ToUTF8(name));
#endif

    Mesh* result = mesh.get();
    m_meshes.emplace(name, std::move(mesh));
    return result;
}

// =========================================================
// Get Mesh
// =========================================================

Mesh* MeshManager::GetMesh(const std::wstring& name) const
{
    auto it = m_meshes.find(name);
    return (it != m_meshes.end()) ? it->second.get() : nullptr;
}

// =========================================================
// Built-in Meshes
// =========================================================

void MeshManager::CreateBuiltinMeshes()
{
    if (m_quad)
        return;

    // -------------------------------------------------
    // Quad (Position + UV)
    // -------------------------------------------------
    struct Vertex
    {
        float x, y, z;
        float u, v;
    };

    static constexpr Vertex quadVertices[] =
    {
        { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, // left-bottom
        { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // right-bottom
        { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f }, // right-top
        { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }, // left-top
    };

    static constexpr uint16 quadIndices[] =
    {
        3, 1, 0,
        2, 1, 3
    };

    m_quad = CreateMesh(
        L"__Builtin_Quad",
        quadVertices,
        sizeof(Vertex),
        4,
        quadIndices,
        6);
}

// =========================================================
// Get Built-in
// =========================================================

Mesh* MeshManager::GetQuad() const
{
    return m_quad;
}

// =========================================================
// Clear
// =========================================================

void MeshManager::Clear()
{
    m_meshes.clear();
    m_quad = nullptr;

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "MeshManager cleared");
#endif
}
