#pragma once

#include <vector>

class GraphicsPSO;
class MaterialInstance;
class Mesh;

/*
    GPUCommand
    -------------------------------------------------
    - GPU 실행 단위
*/
struct GPUCommand
{
    GraphicsPSO* PSO{ nullptr };
    MaterialInstance* MaterialInstance{ nullptr };
    Mesh* Mesh{ nullptr };

    uint32 IndexCount{ 0 };
    uint32 StartIndex{ 0 };
    int32  BaseVertex{ 0 };
};

class CommandBuffer
{
public:
    void Add(const GPUCommand& cmd);
    const std::vector<GPUCommand>& GetCommands() const;

    void Clear();
    bool IsEmpty() const;

private:
    std::vector<GPUCommand> m_commands;
};
