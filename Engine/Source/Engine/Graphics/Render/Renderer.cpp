#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/Renderer.h"
#include "Engine/Graphics/Render/CommandBuffer.h"

#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/Material/MaterialInstance.h"
#include "Engine/Graphics/Mesh/Mesh.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"
#include "Engine/Graphics/Sampler/SamplerManager.h"

// =========================================================
// Constructor / Destructor
// =========================================================

Renderer::Renderer(
    RenderDevice* device,
    SamplerManager* samplerManager)
    : m_device(device)
    , m_samplerManager(samplerManager)
{
    SP_ASSERT(m_device != nullptr);
    SP_ASSERT(m_samplerManager != nullptr);
}

Renderer::~Renderer() = default;

// =========================================================
// Batch execution (legacy / future)
// =========================================================

void Renderer::Execute(const CommandBuffer& commandBuffer)
{
    if (commandBuffer.IsEmpty())
        return;

    ID3D11Device* device = m_device->GetDevice();
    ID3D11DeviceContext* context = m_device->GetImmediateContext();

    SP_ASSERT(device != nullptr);
    SP_ASSERT(context != nullptr);

    for (const GPUCommand& cmd : commandBuffer.GetCommands())
    {
#if defined(_DEBUG)
        SP_ASSERT(cmd.PSO != nullptr);
        SP_ASSERT(cmd.MaterialInstance != nullptr);
        SP_ASSERT(cmd.Mesh != nullptr);
        SP_ASSERT(cmd.IndexCount > 0);
#endif

        cmd.PSO->Bind(context);

        cmd.MaterialInstance->BindResources(
            *m_samplerManager,
            device,
            context);

        cmd.Mesh->Bind(context);

        context->DrawIndexed(
            cmd.IndexCount,
            cmd.StartIndex,
            cmd.BaseVertex);
    }
}

// =========================================================
// Immediate execution (Phase 4 RenderPass)
// =========================================================

void Renderer::ExecuteSingle(
    RenderDevice& device,
    GraphicsPSO* pso,
    MaterialInstance* material,
    Mesh* mesh,
    uint32 indexCount,
    uint32 startIndex,
    int32 baseVertex)
{
#if defined(_DEBUG)
    SP_ASSERT(pso != nullptr);
    SP_ASSERT(material != nullptr);
    SP_ASSERT(mesh != nullptr);
    SP_ASSERT(indexCount > 0);
#endif

    ID3D11Device* d3dDevice = device.GetDevice();
    ID3D11DeviceContext* context = device.GetImmediateContext();

    SP_ASSERT(d3dDevice != nullptr);
    SP_ASSERT(context != nullptr);

    // -------------------------------------------------
    // Bind PSO
    // -------------------------------------------------
    pso->Bind(context);

    // -------------------------------------------------
    // Bind material
    // -------------------------------------------------
    material->BindResources(
        *m_samplerManager,
        d3dDevice,
        context);

    // -------------------------------------------------
    // Bind mesh
    // -------------------------------------------------
    mesh->Bind(context);

    // -------------------------------------------------
    // Draw
    // -------------------------------------------------
    context->DrawIndexed(
        indexCount,
        startIndex,
        baseVertex);
}
