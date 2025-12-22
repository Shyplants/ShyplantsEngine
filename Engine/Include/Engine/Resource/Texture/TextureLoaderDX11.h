#pragma once

#include <string>
#include <wrl/client.h>

#include "Engine/Resource/Texture/TextureDesc.h"

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

class RenderDevice;

/*
    TextureLoaderDX11
    -------------------------------------------------
    - DX11 전용 텍스처 로더
*/
class TextureLoaderDX11
{
public:
    static bool LoadFromFile(
        RenderDevice* device,
        const std::wstring& path,
        const TextureDesc& desc,
        Microsoft::WRL::ComPtr<ID3D11Resource>& outResource,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outSRV);
};
