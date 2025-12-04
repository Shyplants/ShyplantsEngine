#include "Engine/Core/EnginePCH.h"
#include "Engine/Resource/Texture/TextureLoader.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

bool TextureLoader::LoadFromFile(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& path, Microsoft::WRL::ComPtr<ID3D11Resource>& outResource, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outSRV)
{
    HRESULT hr = E_FAIL;

    if (path.find(L".dds") != std::wstring::npos)
    {
        hr = CreateDDSTextureFromFile(device, path.c_str(),
            outResource.GetAddressOf(), outSRV.GetAddressOf());
    }
    else
    {
        hr = CreateWICTextureFromFile(device, context, path.c_str(),
            outResource.GetAddressOf(), outSRV.GetAddressOf());
    }

    if (FAILED(hr))
    {
        OutputDebugStringW((L"TextureLoader failed: " + path + L"\n").c_str());
        return false;
    }

    return true;
}
