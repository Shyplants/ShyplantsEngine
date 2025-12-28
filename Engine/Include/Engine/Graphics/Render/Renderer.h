#pragma once

#include "Common/Types.h"

// Forward declarations
class CommandBuffer;
class RenderDevice;
class SamplerManager;
class GraphicsPSO;
class MaterialInstance;
class Mesh;

struct ID3D11DeviceContext;

/*
    Renderer
    -------------------------------------------------
    - GPU 명령 실행자 (stateless)
    - PSO / MaterialInstance / Mesh 바인딩만 수행
    - RenderPass / World / UI 개념 없음
*/
class Renderer
{
public:
    Renderer(RenderDevice* device, SamplerManager* samplerManager);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

public:
    // =========================================================
    // Batch execution (Phase 3 / Phase 5용)
    // =========================================================
    void Execute(const CommandBuffer& commandBuffer);

public:
    // =========================================================
    // Immediate execution (Phase 4 RenderPass용)
    // =========================================================
    void ExecuteSingle(
        RenderDevice& device,
        GraphicsPSO* pso,
        MaterialInstance* material,
        Mesh* mesh,
        uint32 indexCount,
        uint32 startIndex,
        int32 baseVertex);

private:
    RenderDevice* m_device{ nullptr };
    SamplerManager* m_samplerManager{ nullptr };
};
