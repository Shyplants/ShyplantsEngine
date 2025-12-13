#include "Engine/Core/EnginePCH.h"
#include "Engine/Resource/Texture/TextureLoader.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

bool TextureLoader::LoadFromFile(
    ID3D11Device* device, 
    ID3D11DeviceContext* context, 
    const std::wstring& path,
    TextureColorSpace colorSpace,
    Microsoft::WRL::ComPtr<ID3D11Resource>& outResource, 
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outSRV)
{
    HRESULT hr = E_FAIL;

    if (path.find(L".dds") != std::wstring::npos)
    {
        hr = CreateDDSTextureFromFile(
            device, 
            path.c_str(),
            outResource.GetAddressOf(), 
            outSRV.GetAddressOf());
    }
    else
    {
        // PNG / JPG / BMP µî
        WIC_LOADER_FLAGS flags = WIC_LOADER_DEFAULT;

        if (colorSpace == TextureColorSpace::Linear)
        {
            flags = WIC_LOADER_IGNORE_SRGB;
        }

        hr = CreateWICTextureFromFileEx(
            device,
            context,
            path.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE,
            0,
            0,
            flags,
            outResource.GetAddressOf(),
            outSRV.GetAddressOf()
        );
    }

    if (FAILED(hr))
    {
        OutputDebugStringW((L"TextureLoader failed: " + path + L"\n").c_str());
        return false;
    }

    return true;
}
