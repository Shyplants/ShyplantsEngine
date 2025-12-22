#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Render/RenderSystem.h"
#include "Engine/Graphics/Render/RenderQueue.h"
#include "Engine/Graphics/Render/CommandBuffer.h"
#include "Engine/Graphics/Render/Renderer.h"

#include "Engine/Graphics/Renderer/RenderDevice.h"
#include "Engine/Graphics/PSO/GraphicsPSOManager.h"

// =========================================================
// Constructor / Destructor
// =========================================================

RenderSystem::RenderSystem(
    RenderDevice* device,
    Renderer* renderer)
    : m_device(device)
    , m_renderer(renderer)
{
    SP_ASSERT(m_device != nullptr);
    SP_ASSERT(m_renderer != nullptr);

    m_renderQueue = std::make_unique<RenderQueue>();
    m_commandBuffer = std::make_unique<CommandBuffer>();
}

RenderSystem::~RenderSystem() = default;

// =========================================================
// Frame
// =========================================================

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

    for (const DrawCommand& draw : m_renderQueue->GetCommands())
    {
        // -------------------------------------------------
        // (1) DrawCommand validation
        // -------------------------------------------------
#if defined(_DEBUG)
        SP_ASSERT(draw.IsValid());
#endif
        if (!draw.IsValid())
            continue; // release safety

        // -------------------------------------------------
        // (2) Resolve / create PSO
        // -------------------------------------------------
        GraphicsPSO* pso = psoManager.GetOrCreate(draw.Pipeline);

#if defined(_DEBUG)
        SP_ASSERT(pso != nullptr);
#endif
        if (pso == nullptr)
            continue; // fail-safe (PSO 생성 실패)

        // -------------------------------------------------
        // Build GPU command
        // -------------------------------------------------
        GPUCommand gpu{};
        gpu.PSO = pso;
        gpu.MaterialInstance = draw.MaterialInstance;
        gpu.Mesh = draw.Mesh;
        gpu.IndexCount = draw.IndexCount;
        gpu.StartIndex = draw.StartIndex;
        gpu.BaseVertex = draw.BaseVertex;

        m_commandBuffer->Add(gpu);
    }

    m_renderer->Execute(*m_commandBuffer);
}

void RenderSystem::EndFrame()
{
    SP_ASSERT(m_frameBegun);

    m_device->EndFrame();
    m_frameBegun = false;
}

// =========================================================
// Access
// =========================================================

RenderQueue& RenderSystem::GetRenderQueue()
{
    return *m_renderQueue;
}

const RenderQueue& RenderSystem::GetRenderQueue() const
{
    return *m_renderQueue;
}
