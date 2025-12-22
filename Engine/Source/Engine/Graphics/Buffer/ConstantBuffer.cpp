#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Buffer/ConstantBuffer.h"

// =========================================================
// Constructor / Destructor
// =========================================================

ConstantBuffer::ConstantBuffer(
    uint32 sizeInBytes,
    ConstantBufferUsage usage)
    : m_size(sizeInBytes)
    , m_alignedSize(Align16(sizeInBytes))
    , m_usage(usage)
{
    SP_ASSERT(sizeInBytes > 0);

    // DX11 ConstantBuffer ±Ô¾à: ByteWidth must be multiple of 16
#if defined(_DEBUG)
    SP_ASSERT((m_alignedSize % 16) == 0);
#endif

    m_cpuData.resize(m_alignedSize, 0);
}

ConstantBuffer::~ConstantBuffer() = default;

// =========================================================
// CPU-side update
// =========================================================

void ConstantBuffer::CacheCPUData(const void* data, uint32 size)
{
    SP_ASSERT(data != nullptr);
    SP_ASSERT(size <= m_size);

    memcpy(m_cpuData.data(), data, size);
    m_dirty = true;
}

// =========================================================
// GPU upload
// =========================================================

void ConstantBuffer::UploadIfDirty(
    ID3D11Device* device,
    ID3D11DeviceContext* context)
{
    if (!m_dirty)
        return;

    SP_ASSERT(device != nullptr);
    SP_ASSERT(context != nullptr);

    if (!m_buffer)
    {
        CreateBufferIfNeeded(device);
    }

    if (m_usage == ConstantBufferUsage::Dynamic)
    {
        D3D11_MAPPED_SUBRESOURCE mapped{};
        HRESULT hr = context->Map(
            m_buffer.Get(),
            0,
            D3D11_MAP_WRITE_DISCARD,
            0,
            &mapped);

        SP_ASSERT(SUCCEEDED(hr));
        if (FAILED(hr))
            return;

        memcpy(mapped.pData, m_cpuData.data(), m_alignedSize);
        context->Unmap(m_buffer.Get(), 0);
    }
    else
    {
        context->UpdateSubresource(
            m_buffer.Get(),
            0,
            nullptr,
            m_cpuData.data(),
            0,
            0);
    }

    m_dirty = false;
}

// =========================================================
// Buffer creation
// =========================================================

void ConstantBuffer::CreateBufferIfNeeded(ID3D11Device* device)
{
    SP_ASSERT(device != nullptr);

    if (m_buffer)
        return;

    D3D11_BUFFER_DESC desc{};
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = m_alignedSize;

    if (m_usage == ConstantBufferUsage::Dynamic)
    {
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
    }

    D3D11_SUBRESOURCE_DATA initData{};
    D3D11_SUBRESOURCE_DATA* pInitData = nullptr;

    if (!m_cpuData.empty())
    {
        initData.pSysMem = m_cpuData.data();
        pInitData = &initData;
    }

    HRESULT hr = device->CreateBuffer(
        &desc,
        pInitData,
        m_buffer.GetAddressOf());

    SP_ASSERT(SUCCEEDED(hr));

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "ConstantBuffer created (size=" << m_size
        << ", aligned=" << m_alignedSize << ")");
#endif
}
