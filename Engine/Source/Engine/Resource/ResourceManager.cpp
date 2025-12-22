#include "Engine/PCH/EnginePCH.h"
#include "Engine/Resource/ResourceManager.h"

#include <filesystem>

ResourceManager* ResourceManager::s_instance = nullptr;

// =====================================================
// Lifetime
// =====================================================

void ResourceManager::Create()
{
    SP_ASSERT(s_instance == nullptr);
    s_instance = new ResourceManager();
}

void ResourceManager::Destroy()
{
    delete s_instance;
    s_instance = nullptr;
}

ResourceManager& ResourceManager::Get()
{
    SP_ASSERT(s_instance != nullptr);
    return *s_instance;
}

ResourceManager::~ResourceManager()
{
    UnloadAll();
}

// =====================================================
// Context
// =====================================================

void ResourceManager::SetLoadContext(const ResourceLoadContext& context)
{
    m_context = context;
}

// =====================================================
// Unload
// =====================================================

void ResourceManager::Unload(const std::wstring& path)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto key = NormalizePath(path);
    auto it = m_resources.find(key);
    if (it == m_resources.end())
        return;

    it->second->Unload();
    m_resources.erase(it);
}

void ResourceManager::UnloadAll()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& [_, res] : m_resources)
        res->Unload();

    m_resources.clear();
}

// =====================================================
// Path normalize
// =====================================================

std::wstring ResourceManager::NormalizePath(const std::wstring& path) const
{
    return std::filesystem::weakly_canonical(path).wstring();
}
