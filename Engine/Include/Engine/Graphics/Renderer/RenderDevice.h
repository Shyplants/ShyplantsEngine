#pragma once

#include <wrl/client.h>
#include <d3d11.h>

#include "Common/Types.h"

// Forward declarations
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class VertexShader;

/*
    RenderDevice
    -------------------------------------------------
    - D3D11 Device / Context / SwapChain 包府
    - BackBuffer / DepthStencil 包府
    - DX11 State Object 积己 蒲配府
*/
class RenderDevice
{
public:
    RenderDevice();
    ~RenderDevice();

    RenderDevice(const RenderDevice&) = delete;
    RenderDevice& operator=(const RenderDevice&) = delete;

public:
    // =========================================================
    // Initialize / Shutdown
    // =========================================================
    bool Initialize(void* nativeWindowHandle, uint32 width, uint32 height);
    void Shutdown();

public:
    // =========================================================
    // Frame
    // =========================================================
    void BeginFrame(const float clearColor[4]);
    void EndFrame();

public:
    // =========================================================
    // Resize
    // =========================================================
    void Resize(uint32 width, uint32 height);

public:
    // =========================================================
    // Accessors
    // =========================================================
    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetImmediateContext() const;

    uint32 GetWidth()  const { return m_width; }
    uint32 GetHeight() const { return m_height; }

public:
    // =========================================================
    // Fixed Pipeline State Creation (Factory)
    // =========================================================
    Microsoft::WRL::ComPtr<ID3D11InputLayout>
        CreateSpriteInputLayout(VertexShader* vs) const;

    Microsoft::WRL::ComPtr<ID3D11BlendState>
        CreateAlphaBlendState() const;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState>
        CreateCullNoneRasterizer() const;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>
        CreateDepthReadState() const;

private:
    bool CreateSwapChain(HWND hwnd);
    bool CreateBackBuffer();
    bool CreateDepthStencil();

private:
    uint32 m_width{ 0 };
    uint32 m_height{ 0 };

    Microsoft::WRL::ComPtr<ID3D11Device>        m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>      m_swapChain;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
};
