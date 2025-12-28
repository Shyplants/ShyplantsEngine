#pragma once

#include <memory>

#include "Common/Types.h"

// Forward declarations
class RenderDevice;
class Renderer;
class RenderSystem;

class GraphicsPSOManager;
class PipelineStateLibrary;

class ShaderManager;
class SamplerManager;
class MaterialManager;
class MeshManager;

class Material;
class Mesh;

/*
    GraphicsSubsystem
    -------------------------------------------------
    - 그래픽스 전역 소유자
    - RenderDevice / RenderSystem / Managers 조립
    - Engine lifetime 동안 유지
*/
class GraphicsSubsystem final
{
public:
    GraphicsSubsystem();
    ~GraphicsSubsystem();

    GraphicsSubsystem(const GraphicsSubsystem&) = delete;
    GraphicsSubsystem& operator=(const GraphicsSubsystem&) = delete;

public:
    // =========================================================
    // Lifecycle
    // =========================================================
    bool Initialize(void* nativeWindowHandle, uint32 width, uint32 height);
    void Shutdown();

public:
    // =========================================================
    // Frame
    // =========================================================
    void BeginFrame(const float clearColor[4]);
    void Render();
    void EndFrame();

public:
    // =========================================================
    // Resize
    // =========================================================
    void Resize(uint32 width, uint32 height);

public:
    // =========================================================
    // Accessors
    // =========================================================
    RenderSystem& GetRenderSystem();
    RenderDevice& GetRenderDevice();

    GraphicsPSOManager& GetPSOManager();
    PipelineStateLibrary& GetPipelineLibrary();

    ShaderManager& GetShaderManager();
    SamplerManager& GetSamplerManager();
    MaterialManager& GetMaterialManager();
    MeshManager& GetMeshManager();

    Material* GetDefaultSpriteMaterial() const;
    Mesh* GetDefaultSpriteMesh() const;

private:
    // =========================================================
    // Core rendering
    // =========================================================
    std::unique_ptr<RenderDevice> m_renderDevice;
    std::unique_ptr<Renderer>     m_renderer;
    std::unique_ptr<RenderSystem> m_renderSystem;

private:
    // =========================================================
    // Render infra
    // =========================================================
    std::unique_ptr<GraphicsPSOManager>   m_psoManager;
    std::unique_ptr<PipelineStateLibrary> m_pipelineLibrary;

private:
    // =========================================================
    // Resource managers
    // =========================================================
    std::unique_ptr<ShaderManager>   m_shaderManager;
    std::unique_ptr<SamplerManager>  m_samplerManager;
    std::unique_ptr<MaterialManager> m_materialManager;
    std::unique_ptr<MeshManager>     m_meshManager;
};
