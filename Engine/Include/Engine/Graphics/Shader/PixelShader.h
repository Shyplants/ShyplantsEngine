#pragma once

#include <string>
#include <wrl/client.h>
#include <d3dcommon.h>

// Forward declarations
class RenderDevice;
struct ID3D11PixelShader;

/*
    PixelShader
    -------------------------------------------------
    - DX11 Pixel Shader 래퍼
    - Bytecode 보관 불필요
    - 불변 리소스
*/
class PixelShader
{
public:
    explicit PixelShader(RenderDevice* device);
    ~PixelShader();

    PixelShader(const PixelShader&) = delete;
    PixelShader& operator=(const PixelShader&) = delete;

public:
    // =========================================================
    // Load
    // =========================================================
    bool Load(
        const std::wstring& file,
        const std::string& entry);

public:
    // =========================================================
    // Accessor
    // =========================================================
    ID3D11PixelShader* GetShader() const { return m_shader.Get(); }

private:
    RenderDevice* m_device{ nullptr };

    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
};
