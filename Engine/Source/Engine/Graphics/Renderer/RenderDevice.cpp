#include "Engine/PCH/GraphicsPCH.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"

#include "Engine/Graphics/Shader/VertexShader.h"

#include <dxgi.h>

using Microsoft::WRL::ComPtr;

// =========================================================
// Constructor / Destructor
// =========================================================

RenderDevice::RenderDevice() = default;
RenderDevice::~RenderDevice() = default;

// =========================================================
// Initialize
// =========================================================

bool RenderDevice::Initialize(void* nativeWindowHandle, uint32 width, uint32 height)
{
    HWND hwnd = static_cast<HWND>(nativeWindowHandle);

    SP_ASSERT(hwnd != nullptr);
    SP_ASSERT(width > 0 && height > 0);

    m_width = width;
    m_height = height;

    UINT deviceFlags = 0;
#if defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        deviceFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_device.GetAddressOf(),
        nullptr,
        m_context.GetAddressOf());

    if (FAILED(hr))
    {
        SP_LOG(LogRender, ELogLevel::Fatal,
            "Failed to create D3D11 Device");
        return false;
    }

    if (!CreateSwapChain(hwnd))
        return false;

    if (!CreateBackBuffer())
        return false;

    if (!CreateDepthStencil())
        return false;

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "RenderDevice initialized");
#endif

    return true;
}

// =========================================================
// Shutdown
// =========================================================

void RenderDevice::Shutdown()
{
    m_dsv.Reset();
    m_rtv.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
}

// =========================================================
// Frame
// =========================================================

void RenderDevice::BeginFrame(const float clearColor[4])
{
    SP_ASSERT(m_context != nullptr);
    SP_ASSERT(m_rtv != nullptr);

    m_context->OMSetRenderTargets(
        1,
        m_rtv.GetAddressOf(),
        m_dsv.Get());

    m_context->ClearRenderTargetView(
        m_rtv.Get(),
        clearColor);

    if (m_dsv)
    {
        m_context->ClearDepthStencilView(
            m_dsv.Get(),
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
            1.0f,
            0);
    }

    D3D11_VIEWPORT vp{};
    vp.Width = static_cast<float>(m_width);
    vp.Height = static_cast<float>(m_height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    m_context->RSSetViewports(1, &vp);
}

void RenderDevice::EndFrame()
{
    SP_ASSERT(m_swapChain != nullptr);
    m_swapChain->Present(1, 0);
}

// =========================================================
// Resize
// =========================================================

void RenderDevice::Resize(uint32 width, uint32 height)
{
    if (width == 0 || height == 0)
        return;

    SP_ASSERT(m_context != nullptr);
    SP_ASSERT(m_swapChain != nullptr);

    m_width = width;
    m_height = height;

    m_context->OMSetRenderTargets(0, nullptr, nullptr);

    m_rtv.Reset();
    m_dsv.Reset();

    HRESULT hr = m_swapChain->ResizeBuffers(
        0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    SP_ASSERT(SUCCEEDED(hr));
    if (FAILED(hr))
        return;

    CreateBackBuffer();
    CreateDepthStencil();
}

// =========================================================
// Accessors
// =========================================================

ID3D11Device* RenderDevice::GetDevice() const
{
    return m_device.Get();
}

ID3D11DeviceContext* RenderDevice::GetImmediateContext() const
{
    return m_context.Get();
}

// =========================================================
// Fixed Pipeline State Creation
// =========================================================

ComPtr<ID3D11InputLayout>
RenderDevice::CreateSpriteInputLayout(VertexShader* vs) const
{
    SP_ASSERT(vs != nullptr);
    SP_ASSERT(vs->GetBytecode() != nullptr);

    static const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ComPtr<ID3D11InputLayout> inputLayout;

    HRESULT hr = m_device->CreateInputLayout(
        layout,
        _countof(layout),
        vs->GetBytecode()->GetBufferPointer(),
        vs->GetBytecodeSize(),
        inputLayout.GetAddressOf());

    SP_ASSERT(SUCCEEDED(hr));
    return inputLayout;
}

ComPtr<ID3D11BlendState>
RenderDevice::CreateAlphaBlendState() const
{
    D3D11_BLEND_DESC desc{};
    desc.RenderTarget[0].BlendEnable = TRUE;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;

    ComPtr<ID3D11BlendState> state;
    HRESULT hr = m_device->CreateBlendState(&desc, state.GetAddressOf());

    SP_ASSERT(SUCCEEDED(hr));
    return state;
}

ComPtr<ID3D11RasterizerState>
RenderDevice::CreateCullNoneRasterizer() const
{
    D3D11_RASTERIZER_DESC desc{};
    desc.FillMode = D3D11_FILL_SOLID;
    desc.CullMode = D3D11_CULL_NONE;
    desc.DepthClipEnable = TRUE;

    ComPtr<ID3D11RasterizerState> state;
    HRESULT hr = m_device->CreateRasterizerState(&desc, state.GetAddressOf());

    SP_ASSERT(SUCCEEDED(hr));
    return state;
}

ComPtr<ID3D11DepthStencilState>
RenderDevice::CreateDepthReadState() const
{
    D3D11_DEPTH_STENCIL_DESC desc{};
    desc.DepthEnable = TRUE;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    ComPtr<ID3D11DepthStencilState> state;
    HRESULT hr = m_device->CreateDepthStencilState(&desc, state.GetAddressOf());

    SP_ASSERT(SUCCEEDED(hr));
    return state;
}

// =========================================================
// Internal Helpers
// =========================================================

bool RenderDevice::CreateSwapChain(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 2;
    desc.SampleDesc.Count = 1;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Scaling = DXGI_SCALING_NONE;
    desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc{};
    fsDesc.Windowed = TRUE;

    ComPtr<IDXGIDevice> dxgiDevice;
    m_device.As(&dxgiDevice);

    ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);

    ComPtr<IDXGIFactory2> factory;
    adapter->GetParent(IID_PPV_ARGS(&factory));

    ComPtr<IDXGISwapChain1> swapChain1;
    HRESULT hr = factory->CreateSwapChainForHwnd(
        m_device.Get(),
        hwnd,
        &desc,
        &fsDesc,
        nullptr,
        &swapChain1);

    if (FAILED(hr))
        return false;

    swapChain1.As(&m_swapChain);
    return true;
}

bool RenderDevice::CreateBackBuffer()
{
    ComPtr<ID3D11Texture2D> backBuffer;

    HRESULT hr = m_swapChain->GetBuffer(
        0,
        IID_PPV_ARGS(&backBuffer));

    if (FAILED(hr))
        return false;

    hr = m_device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        m_rtv.GetAddressOf());

    return SUCCEEDED(hr);
}

bool RenderDevice::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ComPtr<ID3D11Texture2D> depthTex;
    HRESULT hr = m_device->CreateTexture2D(
        &desc, nullptr, depthTex.GetAddressOf());

    if (FAILED(hr))
        return false;

    hr = m_device->CreateDepthStencilView(
        depthTex.Get(), nullptr, m_dsv.GetAddressOf());

    return SUCCEEDED(hr);
}
