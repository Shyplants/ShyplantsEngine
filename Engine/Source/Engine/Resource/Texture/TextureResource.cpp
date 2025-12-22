#include "Engine/PCH/GraphicsPCH.h"

#include "Engine/Resource/Texture/TextureResource.h"
#include "Engine/Resource/ResourceLoadContext.h"
#include "Engine/Graphics/Renderer/RenderDevice.h"
#include "Engine/Resource/Texture/TextureLoaderDX11.h"

#include "Engine/Core/Utils/StringUtils.h"

using Microsoft::WRL::ComPtr;

// =====================================================
// Constructor / Destructor
// =====================================================

TextureResource::TextureResource()
    : Resource(ResourceType::Texture)
{
}

TextureResource::~TextureResource()
{
    Unload();
}

// =====================================================
// Builder-only
// =====================================================

void TextureResource::ApplyDesc(const TextureDesc& desc)
{
    SP_ASSERT(!IsLoaded());
    SP_ASSERT(!m_descApplied);

    m_desc = desc;
    m_descApplied = true;
}

// =====================================================
// Resource Interface
// =====================================================

bool TextureResource::Load(const ResourceLoadContext& context)
{
    SP_ASSERT(m_descApplied &&
        "TextureResource must be created via TextureBuilder");

    SP_ASSERT(context.renderDevice != nullptr);
    SP_ASSERT(!GetPath().empty());

    return CreateFromFile(context);
}

void TextureResource::Unload()
{
    if (!IsLoaded())
        return;

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "Texture unloaded: " << StringUtils::ToUTF8(GetPath()));
#endif

    m_srv.Reset();
    m_texture.Reset();

    m_width = 0;
    m_height = 0;

    SetLoaded(false);
}

// =====================================================
// Internal
// =====================================================

bool TextureResource::CreateFromFile(const ResourceLoadContext& context)
{
    Unload();

    ComPtr<ID3D11Resource> resource;

    bool ok = TextureLoaderDX11::LoadFromFile(
        context.renderDevice,
        GetPath(),
        m_desc,
        resource,
        m_srv);

    if (!ok || !resource)
        return false;

    HRESULT hr = resource.As(&m_texture);
    if (FAILED(hr))
        return false;

    D3D11_TEXTURE2D_DESC desc{};
    m_texture->GetDesc(&desc);

    m_width = desc.Width;
    m_height = desc.Height;

    SetLoaded(true);

#if defined(_DEBUG)
    SP_LOG(LogRender, ELogLevel::Trace,
        "Texture loaded: " << StringUtils::ToUTF8(GetPath())
        << " (" << m_width << "x" << m_height << ")");
#endif

    return true;
}
