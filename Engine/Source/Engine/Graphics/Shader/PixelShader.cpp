#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Graphics/Shader/PixelShader.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"

#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

// =========================================================
// Constructor / Destructor
// =========================================================

PixelShader::PixelShader(RenderDevice* device)
    : m_device(device)
{
    SP_ASSERT(m_device != nullptr);
}

PixelShader::~PixelShader() = default;

// =========================================================
// Load
// =========================================================

bool PixelShader::Load(
    const std::wstring& file,
    const std::string& entry)
{
    SP_ASSERT(!file.empty());
    SP_ASSERT(!entry.empty());

    ComPtr<ID3DBlob> shaderBlob;
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
        "ps_5_0",
        compileFlags,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            SP_LOG(LogRender, ELogLevel::Fatal,
                "PixelShader compile error:\n"
                << reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
        }
        return false;
    }

    hr = m_device->GetDevice()->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        m_shader.GetAddressOf());

    if (FAILED(hr))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to create PixelShader");
        return false;
    }

    return true;
}
