#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Subsystem/GraphicsSubsystem.h"

// Core
#include "Engine/Graphics/Renderer/RenderDevice.h"
#include "Engine/Graphics/Render/Renderer.h"
#include "Engine/Graphics/Render/RenderSystem.h"

// Pipeline
#include "Engine/Graphics/PSO/GraphicsPSOManager.h"
#include "Engine/Graphics/PSO/PipelineStateLibrary.h"

// Managers
#include "Engine/Graphics/Shader/ShaderManager.h"
#include "Engine/Graphics/Sampler/SamplerManager.h"
#include "Engine/Graphics/Material/MaterialManager.h"
#include "Engine/Graphics/Mesh/MeshManager.h"

// Resources
#include "Engine/Graphics/Material/Material.h"
#include "Engine/Graphics/Mesh/Mesh.h"
#include "Engine/Graphics/Render/RenderDefaults.h"

// =========================================================
// Constructor / Destructor
// =========================================================

GraphicsSubsystem::GraphicsSubsystem() = default;
GraphicsSubsystem::~GraphicsSubsystem() = default;

// =========================================================
// Initialize
// =========================================================

bool GraphicsSubsystem::Initialize(
    void* nativeWindowHandle,
    uint32 width,
    uint32 height)
{
    SP_ASSERT(nativeWindowHandle != nullptr);
    SP_ASSERT(width > 0 && height > 0);

    // -------------------------------------------------
    // RenderDevice
    // -------------------------------------------------
    m_renderDevice = std::make_unique<RenderDevice>();
    if (!m_renderDevice->Initialize(nativeWindowHandle, width, height))
    {
        Shutdown();
        return false;
    }

    // -------------------------------------------------
    // Resource managers
    // -------------------------------------------------
    m_shaderManager = std::make_unique<ShaderManager>(m_renderDevice.get());
    m_samplerManager = std::make_unique<SamplerManager>(m_renderDevice.get());
    m_materialManager = std::make_unique<MaterialManager>();
    m_meshManager = std::make_unique<MeshManager>(m_renderDevice->GetDevice());

    // -------------------------------------------------
    // Render infra
    // -------------------------------------------------
    m_pipelineLibrary = std::make_unique<PipelineStateLibrary>();
    m_psoManager = std::make_unique<GraphicsPSOManager>(m_pipelineLibrary.get());

    // -------------------------------------------------
    // Renderer (GPU executor)
    // -------------------------------------------------
    m_renderer = std::make_unique<Renderer>(
        m_renderDevice.get(),
        m_samplerManager.get());

    // -------------------------------------------------
    // Built-in shaders
    // -------------------------------------------------
    m_shaderManager->LoadVertexShader(
        L"SpriteVS",
        L"../Shaders/SpriteVS.hlsl",
        "main");

    m_shaderManager->LoadPixelShader(
        L"SpritePS",
        L"../Shaders/SpritePS.hlsl",
        "main");

    // -------------------------------------------------
    // Built-in samplers
    // -------------------------------------------------
    m_samplerManager->Initialize();

    // -------------------------------------------------
    // Pipeline states
    // -------------------------------------------------
    m_pipelineLibrary->Initialize(
        *m_shaderManager,
        *m_renderDevice);

    // -------------------------------------------------
    // RenderSystem (Phase 3)
    // -------------------------------------------------
    m_renderSystem = std::make_unique<RenderSystem>(
        m_renderDevice.get(),
        m_renderer.get());

    // -------------------------------------------------
    // Built-in resources
    // -------------------------------------------------
    m_meshManager->CreateBuiltinMeshes();
    m_materialManager->CreateBuiltinMaterials(*m_pipelineLibrary);

    // -------------------------------------------------
    // Render defaults
    // -------------------------------------------------
    Material* defaultMaterial = GetDefaultSpriteMaterial();
    Mesh* defaultMesh = GetDefaultSpriteMesh();
    Mesh* defaultUIMesh = GetDefaultUISpriteMesh();

#if defined(_DEBUG)
    SP_ASSERT(defaultMaterial != nullptr);
    SP_ASSERT(defaultMesh != nullptr);
#endif

    RenderDefaults defaults{};
    defaults.SpriteMaterial = defaultMaterial;
    defaults.SpriteMesh = defaultMesh;
    defaults.UISpriteMesh = defaultUIMesh;

    m_renderSystem->SetDefaults(defaults);

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "GraphicsSubsystem initialized (Phase 3)");
#endif

    return true;
}

// =========================================================
// Shutdown
// =========================================================

void GraphicsSubsystem::Shutdown()
{
    m_meshManager.reset();
    m_materialManager.reset();
    m_samplerManager.reset();
    m_shaderManager.reset();

    m_pipelineLibrary.reset();
    m_psoManager.reset();

    m_renderSystem.reset();
    m_renderer.reset();

    if (m_renderDevice)
    {
        m_renderDevice->Shutdown();
        m_renderDevice.reset();
    }
}

// =========================================================
// Frame
// =========================================================

void GraphicsSubsystem::BeginFrame(const float clearColor[4])
{
    SP_ASSERT(m_renderSystem != nullptr);
    m_renderSystem->BeginFrame(clearColor);
}

void GraphicsSubsystem::Render()
{
    SP_ASSERT(m_renderSystem != nullptr);
    SP_ASSERT(m_psoManager != nullptr);

    m_renderSystem->Render(*m_psoManager);
}

void GraphicsSubsystem::EndFrame()
{
    SP_ASSERT(m_renderSystem != nullptr);
    m_renderSystem->EndFrame();
}

// =========================================================
// Resize
// =========================================================

void GraphicsSubsystem::Resize(uint32 width, uint32 height)
{
    if (m_renderDevice)
        m_renderDevice->Resize(width, height);
}

// =========================================================
// Accessors
// =========================================================

RenderSystem& GraphicsSubsystem::GetRenderSystem()
{
    SP_ASSERT(m_renderSystem != nullptr);
    return *m_renderSystem;
}

RenderDevice& GraphicsSubsystem::GetRenderDevice()
{
    SP_ASSERT(m_renderDevice != nullptr);
    return *m_renderDevice;
}

GraphicsPSOManager& GraphicsSubsystem::GetPSOManager()
{
    SP_ASSERT(m_psoManager != nullptr);
    return *m_psoManager;
}

PipelineStateLibrary& GraphicsSubsystem::GetPipelineLibrary()
{
    SP_ASSERT(m_pipelineLibrary != nullptr);
    return *m_pipelineLibrary;
}

ShaderManager& GraphicsSubsystem::GetShaderManager()
{
    SP_ASSERT(m_shaderManager != nullptr);
    return *m_shaderManager;
}

SamplerManager& GraphicsSubsystem::GetSamplerManager()
{
    SP_ASSERT(m_samplerManager != nullptr);
    return *m_samplerManager;
}

MaterialManager& GraphicsSubsystem::GetMaterialManager()
{
    SP_ASSERT(m_materialManager != nullptr);
    return *m_materialManager;
}

MeshManager& GraphicsSubsystem::GetMeshManager()
{
    SP_ASSERT(m_meshManager != nullptr);
    return *m_meshManager;
}

Material* GraphicsSubsystem::GetDefaultSpriteMaterial() const
{
    return m_materialManager
        ? m_materialManager->GetMaterial(L"SpriteAlpha")
        : nullptr;
}

Mesh* GraphicsSubsystem::GetDefaultSpriteMesh() const
{
    return m_meshManager
        ? m_meshManager->GetQuad()
        : nullptr;
}

Mesh* GraphicsSubsystem::GetDefaultUISpriteMesh() const
{
    return m_meshManager
        ? m_meshManager->GetUIQuad()
        : nullptr;
}
