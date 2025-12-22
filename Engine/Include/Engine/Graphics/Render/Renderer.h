#pragma once

class CommandBuffer;
class RenderDevice;
class SamplerManager;

struct ID3D11DeviceContext;

/*
    Renderer
    -------------------------------------------------
    - GPU 명령 실행자
    - PSO / MaterialInstance / Mesh 바인딩만 수행
*/
class Renderer
{
public:
    Renderer(RenderDevice* device, SamplerManager* samplerManager);
    ~Renderer();

    void Execute(const CommandBuffer& commandBuffer);

private:
    RenderDevice* m_device{ nullptr };
    SamplerManager* m_samplerManager{ nullptr };
};
