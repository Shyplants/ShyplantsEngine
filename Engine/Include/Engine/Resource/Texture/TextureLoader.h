#pragma once

#include <string>
#include <wrl/client.h>
#include <d3d11.h>

class TextureLoader
{
public:
    static bool LoadFromFile(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        const std::wstring& path,
        Microsoft::WRL::ComPtr<ID3D11Resource>& outResource,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outSRV);
};