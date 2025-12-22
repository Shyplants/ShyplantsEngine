#pragma once

#include <memory>
#include <string>

#include "Engine/Resource/Texture/TextureDesc.h"
#include "Engine/Resource/Texture/TextureResource.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Core/Assert.h"

/*
    TextureBuilder
    -------------------------------------------------
    - TextureResource 생성 전용 Builder
    - 옵션 → 검증 → 로드 순서 보장
*/
class TextureBuilder
{
public:
    TextureBuilder() = default;

public:
    TextureBuilder& FromFile(const std::wstring& path)
    {
        m_path = path;
        return *this;
    }

    TextureBuilder& ColorSpace(TextureColorSpace cs)
    {
        m_desc.colorSpace = cs;
        return *this;
    }

public:
    std::shared_ptr<TextureResource> Build()
    {
        SP_ASSERT(!m_path.empty());

        return ResourceManager::Get()
            .LoadWithSetup<TextureResource>(
                m_path,
                [&](TextureResource& resource)
                {
                    resource.ApplyDesc(m_desc);
                });
    }

private:
    std::wstring m_path;
    TextureDesc  m_desc;
};
