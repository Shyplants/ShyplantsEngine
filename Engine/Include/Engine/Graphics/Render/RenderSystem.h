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

/*
    RenderSystem
    -------------------------------------------------
    - 프레임 단위 렌더링 조율자
    - RenderQueue → CommandBuffer → Renderer
*/
class RenderSystem
{
public:
    RenderSystem(
        RenderDevice* device,
        Renderer* renderer);
    ~RenderSystem();

    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

public:
    // =========================================================
    // Frame
    // =========================================================
    void BeginFrame(const float clearColor[4]);
    void Render(GraphicsPSOManager& psoManager);
    void EndFrame();

public:
    // =========================================================
    // Access
    // =========================================================
    RenderQueue& GetRenderQueue();
    const RenderQueue& GetRenderQueue() const;

public:
    void SetDefaults(const RenderDefaults& defaults) { m_defaults = defaults; }
    const RenderDefaults& GetDefaults() const { return m_defaults; }

private:
    RenderDevice* m_device{ nullptr };
    Renderer* m_renderer{ nullptr };

    std::unique_ptr<RenderQueue>   m_renderQueue;
    std::unique_ptr<CommandBuffer> m_commandBuffer;

    bool m_frameBegun{ false };
    RenderDefaults m_defaults;
};
