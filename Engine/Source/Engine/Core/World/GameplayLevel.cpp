#include "Engine/PCH/EnginePCH.h"

#include "Engine/Core/World/GameplayLevel.h"
#include "Engine/Core/World/LevelResourceSet.h"

GameplayLevel::GameplayLevel() 
    : Level()
{
}

GameplayLevel::~GameplayLevel() = default;

// =====================================================
// Lifecycle
// =====================================================

void GameplayLevel::OnEnter(World& world)
{
    Level::OnEnter(world);

    if (m_resources)
    {
        m_resources->Load();
    }
}

void GameplayLevel::OnExit(World& world)
{
    // Level 교체 시
    Level::OnExit(world);
}

void GameplayLevel::Shutdown()
{
    // 리소스를 먼저 내린다
    if (m_resources)
    {
        m_resources->Unload();
        m_resources.reset();
    }

    Level::Shutdown();
}

// =====================================================
// ResourceSet
// =====================================================

void GameplayLevel::SetResourceSet(std::unique_ptr<LevelResourceSet> resources)
{
    m_resources = std::move(resources);
}

LevelResourceSet* GameplayLevel::GetResourceSet() const
{
    return m_resources.get();
}
