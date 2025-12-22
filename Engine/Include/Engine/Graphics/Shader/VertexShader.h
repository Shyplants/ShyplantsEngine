#pragma once

#include <string>
#include <wrl/client.h>
#include <d3dcommon.h>

// Forward declarations
class RenderDevice;
struct ID3D11VertexShader;

/*
    VertexShader
    -------------------------------------------------
    - DX11 Vertex Shader 래퍼
    - Shader bytecode 보관 (InputLayout 생성용)
    - 불변 리소스
*/
class VertexShader
{
public:
    explicit VertexShader(RenderDevice* device);
    ~VertexShader();

    VertexShader(const VertexShader&) = delete;
    VertexShader& operator=(const VertexShader&) = delete;

public:
    // =========================================================
    // Load
    // =========================================================
    bool Load(
        const std::wstring& file,
        const std::string& entry);

public:
    // =========================================================
    // Accessors
    // =========================================================
    ID3D11VertexShader* GetShader() const { return m_shader.Get(); }
    ID3DBlob* GetBytecode() const { return m_bytecode.Get(); }
    size_t GetBytecodeSize() const;

private:
    RenderDevice* m_device{ nullptr };

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
    Microsoft::WRL::ComPtr<ID3DBlob>           m_bytecode;
};
