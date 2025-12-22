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
// Execute
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
        // -------------------------------------------------
        // Fail-fast validation (debug only)
        // -------------------------------------------------
        SP_ASSERT(cmd.PSO != nullptr);
        SP_ASSERT(cmd.MaterialInstance != nullptr);
        SP_ASSERT(cmd.Mesh != nullptr);
        SP_ASSERT(cmd.IndexCount > 0);
#endif

        // -------------------------------------------------
        // Bind PSO (pipeline state)
        // -------------------------------------------------
        cmd.PSO->Bind(context);

        // -------------------------------------------------
        // Bind material resources
        // -------------------------------------------------
        cmd.MaterialInstance->BindResources(
            *m_samplerManager,
            device,
            context);

        // -------------------------------------------------
        // Bind mesh buffers
        // -------------------------------------------------
        cmd.Mesh->Bind(context);

        // -------------------------------------------------
        // Draw
        // -------------------------------------------------
        context->DrawIndexed(
            cmd.IndexCount,
            cmd.StartIndex,
            cmd.BaseVertex);
    }
}
