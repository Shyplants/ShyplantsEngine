#include "ClientPCH/ClientPCH.h"

#include "Game/Levels/DemoLevelResources.h"

#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/TextureBuilder.h"
#include "Engine/Resource/Texture/TextureResource.h"

DemoLevelResources::~DemoLevelResources() = default;

void DemoLevelResources::Load()
{
    auto& resourceManager = ResourceManager::Get();

    auto texture = resourceManager.Get<TextureResource>(L"../Resources/test.png");
    if (!texture)
    {
        texture = TextureBuilder()
            .FromFile(L"../Resources/test.png")
            .ColorSpace(TextureColorSpace::Linear)
            .Build();
    }

    SP_ASSERT(texture != nullptr);
}

void DemoLevelResources::Unload()
{

}
