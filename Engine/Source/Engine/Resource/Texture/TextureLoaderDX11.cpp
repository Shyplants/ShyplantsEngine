#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Resource/Texture/TextureLoaderDX11.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"

#include "Engine/Core/Utils/StringUtils.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include <filesystem>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

bool TextureLoaderDX11::LoadFromFile(
    RenderDevice* device,
    const std::wstring& path,
    const TextureDesc& desc,
    ComPtr<ID3D11Resource>& outResource,
    ComPtr<ID3D11ShaderResourceView>& outSRV)
{
    SP_ASSERT(device != nullptr);

    HRESULT hr = E_FAIL;
    auto ext = std::filesystem::path(path).extension().wstring();

    if (_wcsicmp(ext.c_str(), L".dds") == 0)
    {
        hr = CreateDDSTextureFromFile(
            device->GetDevice(),
            path.c_str(),
            outResource.GetAddressOf(),
            outSRV.GetAddressOf());
    }
    else
    {
        WIC_LOADER_FLAGS flags = WIC_LOADER_DEFAULT;

        if (desc.colorSpace == TextureColorSpace::Linear)
        {
            flags = WIC_LOADER_IGNORE_SRGB;
        }

        hr = CreateWICTextureFromFileEx(
            device->GetDevice(),
            device->GetImmediateContext(),
            path.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE,
            0,
            0,
            flags,
            outResource.GetAddressOf(),
            outSRV.GetAddressOf());
    }

    if (FAILED(hr))
    {
        SP_LOG(LogRender, ELogLevel::Error,
            "Texture load failed: " << StringUtils::ToUTF8(path));
        return false;
    }

    return true;
}
