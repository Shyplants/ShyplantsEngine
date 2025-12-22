#pragma once

#include <wrl/client.h>
#include <type_traits>
#include <vector>
#include <cstdint>

#include "Common/Types.h"
#include "Engine/Graphics/Buffer/ConstantBufferUsage.h"

// Forward declarations
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

/*
    ConstantBuffer
    -------------------------------------------------
    - D3D11 Constant Buffer 래퍼
    - CPU → GPU 데이터 업로드 책임
*/
class ConstantBuffer
{
public:
    ConstantBuffer(
        uint32 sizeInBytes,
        ConstantBufferUsage usage);

    ~ConstantBuffer();

public:
    // =========================================================
    // CPU-side update
    // =========================================================
    void CacheCPUData(const void* data, uint32 size);

    template<typename T>
    void CacheCPUData(const T& data)
    {
        static_assert(std::is_standard_layout_v<T>,
            "ConstantBuffer data must be standard-layout");
        CacheCPUData(&data, sizeof(T));
    }

public:
    // =========================================================
    // GPU upload (Renderer only)
    // =========================================================
    void UploadIfDirty(
        ID3D11Device* device,
        ID3D11DeviceContext* context);

public:
    ID3D11Buffer* GetBuffer() const { return m_buffer.Get(); }

private:
    // =========================================================
    // Internal
    // =========================================================
    void CreateBufferIfNeeded(ID3D11Device* device);

    static uint32 Align16(uint32 size)
    {
        return (size + 15u) & ~15u;
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

    std::vector<uint8> m_cpuData;
    bool m_dirty{ false };

    uint32 m_size{ 0 };        // logical size
    uint32 m_alignedSize{ 0 }; // GPU buffer size (16-byte aligned)

    ConstantBufferUsage m_usage{};
};
