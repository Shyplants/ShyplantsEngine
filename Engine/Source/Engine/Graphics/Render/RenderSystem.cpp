#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/RenderSystem.h"
#include "Engine/Graphics/Render/RenderQueue.h"
#include "Engine/Graphics/Render/CommandBuffer.h"
#include "Engine/Graphics/Render/Renderer.h"

#include "Engine/Graphics/Renderer/RenderDevice.h"
#include "Engine/Graphics/PSO/GraphicsPSOManager.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/Render/DrawCommand.h"

// =====================================================
// Constructor / Destructor
// =====================================================

RenderSystem::RenderSystem(
    RenderDevice* device,
    Renderer* renderer)
    : m_device(device)
    , m_renderer(renderer)
{
    SP_ASSERT(m_device);
    SP_ASSERT(m_renderer);

    m_renderQueue = std::make_unique<RenderQueue>();
    m_commandBuffer = std::make_unique<CommandBuffer>();
}

RenderSystem::~RenderSystem() = default;

// =====================================================
// Frame lifecycle
// =====================================================

void RenderSystem::BeginFrame(const float clearColor[4])
{
    SP_ASSERT(!m_frameBegun);

    m_renderQueue->Clear();
    m_commandBuffer->Clear();

    m_device->BeginFrame(clearColor);
    m_frameBegun = true;
}

void RenderSystem::Render(GraphicsPSOManager& psoManager)
{
    SP_ASSERT(m_frameBegun);

    // -------------------------------------------------
    // World Pass
    // -------------------------------------------------
    ExecutePass(
        m_renderQueue->GetWorldPass(),
        psoManager);

    // -------------------------------------------------
    // Screen Pass (UI)
    // -------------------------------------------------
    ExecutePass(
        m_renderQueue->GetScreenPass(),
        psoManager);

    // -------------------------------------------------
    // Debug Pass
    // -------------------------------------------------
    ExecutePass(
        m_renderQueue->GetDebugPass(),
        psoManager);
}

void RenderSystem::EndFrame()
{
    SP_ASSERT(m_frameBegun);

    m_device->EndFrame();
    m_frameBegun = false;
}

// =====================================================
// Submission
// =====================================================

RenderQueue& RenderSystem::GetRenderQueue()
{
    return *m_renderQueue;
}

const RenderQueue& RenderSystem::GetRenderQueue() const
{
    return *m_renderQueue;
}

// =====================================================
// Defaults
// =====================================================

void RenderSystem::SetDefaults(const RenderDefaults& defaults)
{
    m_defaults = defaults;
}

const RenderDefaults& RenderSystem::GetDefaults() const
{
    return m_defaults;
}

// =====================================================
// UI Projection
// =====================================================

void RenderSystem::SetUIProjection(
    const DirectX::XMMATRIX& projection)
{
    m_uiProjection = projection;
}

const DirectX::XMMATRIX&
RenderSystem::GetUIProjection() const
{
    return m_uiProjection;
}

// =====================================================
// Pass execution
// =====================================================

void RenderSystem::ExecutePass(
    const std::vector<DrawCommand>& drawCommands,
    GraphicsPSOManager& psoManager)
{
    if (drawCommands.empty())
        return;

    m_commandBuffer->Clear();

    for (const DrawCommand& draw : drawCommands)
    {
#if defined(_DEBUG)
        SP_ASSERT(draw.IsValid());
#endif
        if (!draw.IsValid())
            continue;

        GraphicsPSO* pso =
            psoManager.GetOrCreate(draw.Pipeline);

        if (!pso)
            continue;

        GPUCommand gpu{};
        gpu.PSO = pso;
        gpu.MaterialInstance = draw.MaterialInstance;
        gpu.Mesh = draw.Mesh;
        gpu.IndexCount = draw.IndexCount;
        gpu.StartIndex = draw.StartIndex;
        gpu.BaseVertex = draw.BaseVertex;

        m_commandBuffer->Add(gpu);
    }

    if (!m_commandBuffer->IsEmpty())
    {
        m_renderer->Execute(*m_commandBuffer);
    }
}
