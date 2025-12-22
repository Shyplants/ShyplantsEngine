#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Shader/VertexShader.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"

#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

// =========================================================
// Constructor / Destructor
// =========================================================

VertexShader::VertexShader(RenderDevice* device)
    : m_device(device)
{
    SP_ASSERT(m_device != nullptr);
}

VertexShader::~VertexShader() = default;

// =========================================================
// Load
// =========================================================

bool VertexShader::Load(
    const std::wstring& file,
    const std::string& entry)
{
    SP_ASSERT(!file.empty());
    SP_ASSERT(!entry.empty());

    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompileFromFile(
        file.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entry.c_str(),
        "vs_5_0",
        compileFlags,
        0,
        m_bytecode.GetAddressOf(),
        errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            SP_LOG(LogRender, ELogLevel::Fatal,
                "VertexShader compile error:\n"
                << reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
        }
        return false;
    }

    hr = m_device->GetDevice()->CreateVertexShader(
        m_bytecode->GetBufferPointer(),
        m_bytecode->GetBufferSize(),
        nullptr,
        m_shader.GetAddressOf());

    if (FAILED(hr))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to create VertexShader");
        return false;
    }

    return true;
}

// =========================================================
// Bytecode
// =========================================================

size_t VertexShader::GetBytecodeSize() const
{
    return m_bytecode
        ? static_cast<size_t>(m_bytecode->GetBufferSize())
        : 0;
}
