#pragma once

#include <memory>

#include "Common/Types.h"
#include "Engine/Graphics/Render/RenderDefaults.h"

// Forward declarations
class RenderDevice;
class Renderer;
class RenderQueue;
class CommandBuffer;
class GraphicsPSOManager;
struct DrawCommand;

/*
    RenderSystem (Phase 4)
    -------------------------------------------------
    - 명시적 RenderPass 기반 렌더링
    - Pass 순서: World → Screen → Debug
    - RenderQueue는 Pass별 DrawCommand 제공
*/
class RenderSystem final
{
public:
    RenderSystem(
        RenderDevice* device,
        Renderer* renderer);
    ~RenderSystem();

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

public:
    // =====================================================
    // Frame lifecycle
    // =====================================================
    void BeginFrame(const float clearColor[4]);
    void Render(GraphicsPSOManager& psoManager);
    void EndFrame();

public:
    // =====================================================
    // Submission
    // =====================================================
    RenderQueue& GetRenderQueue();
    const RenderQueue& GetRenderQueue() const;

public:
    // =====================================================
    // Defaults
    // =====================================================
    void SetDefaults(const RenderDefaults& defaults);
    const RenderDefaults& GetDefaults() const;

private:
    // =====================================================
    // Pass execution
    // =====================================================
    void ExecutePass(
        const std::vector<DrawCommand>& drawCommands,
        GraphicsPSOManager& psoManager);

private:
    RenderDevice* m_device{ nullptr };
    Renderer* m_renderer{ nullptr };

    std::unique_ptr<RenderQueue>   m_renderQueue;
    std::unique_ptr<CommandBuffer> m_commandBuffer;

    bool m_frameBegun{ false };
    RenderDefaults m_defaults;
};
