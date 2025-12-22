#pragma once

#include <wrl/client.h>

#include "Engine/Resource/Resource.h"
#include "Engine/Resource/Texture/TextureDesc.h"
#include "Common/Types.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ResourceLoadContext;

class TextureBuilder;

/*
    TextureResource
    -------------------------------------------------
    - GPU Texture Resource (DX11)
    - TextureBuilder를 통해서만 완성 가능
*/
class TextureResource final : public Resource
{
public:
    TextureResource();
    ~TextureResource() override;

    TextureResource(const TextureResource&) = delete;
    TextureResource& operator=(const TextureResource&) = delete;

public:
    // =====================================================
    // Resource Interface
    // =====================================================
    bool Load(const ResourceLoadContext& context) override;
    void Unload() override;

public:
    // =====================================================
    // Accessors
    // =====================================================
    ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }
    ID3D11Texture2D* GetTexture() const { return m_texture.Get(); }

    uint32 GetWidth()  const { return m_width; }
    uint32 GetHeight() const { return m_height; }

private:
    // =====================================================
    // Builder-only
    // =====================================================
    friend class TextureBuilder;
    void ApplyDesc(const TextureDesc& desc);

private:
    bool CreateFromFile(const ResourceLoadContext& context);

private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

    uint32 m_width{ 0 };
    uint32 m_height{ 0 };

    TextureDesc m_desc{};
    bool m_descApplied{ false };
};
