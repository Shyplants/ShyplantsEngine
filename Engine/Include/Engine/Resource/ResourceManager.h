#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>

#include "Engine/Resource/Resource.h"
#include "Engine/Resource/ResourceLoadContext.h"

class ResourceManager
{
public:
    static void Create();
    static void Destroy();
    static ResourceManager& Get();

private:
    ResourceManager() = default;
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

public:
    // =====================================================
    // Context
    // =====================================================
    void SetLoadContext(const ResourceLoadContext& context);

public:
    // =====================================================
    // Get (Reuse Only)
    // =====================================================
    template<typename T>
    std::shared_ptr<T> Get(const std::wstring& path)
    {
        static_assert(std::is_base_of_v<Resource, T>,
            "T must derive from Resource");

        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = NormalizePath(path);
        auto it = m_resources.find(key);

        if (it == m_resources.end())
            return nullptr;

        return std::static_pointer_cast<T>(it->second);
    }

public:
    // =====================================================
    // Load (Simple)
    // =====================================================
    template<typename T>
    std::shared_ptr<T> Load(const std::wstring& path)
    {
        static_assert(std::is_base_of_v<Resource, T>,
            "T must derive from Resource");

        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = NormalizePath(path);

        auto it = m_resources.find(key);
        if (it != m_resources.end())
            return std::static_pointer_cast<T>(it->second);

        auto resource = std::make_shared<T>();
        resource->Initialize(key);

        if (!resource->Load(m_context))
            return nullptr;

        m_resources.emplace(key, resource);
        return resource;
    }

    // =====================================================
    // Load with Setup (Builder support)
    // - 최초 1회만 허용
    // =====================================================
    template<typename T, typename SetupFn>
    std::shared_ptr<T> LoadWithSetup(
        const std::wstring& path,
        SetupFn&& setupFn)
    {
        static_assert(std::is_base_of_v<Resource, T>,
            "T must derive from Resource");

        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = NormalizePath(path);

        auto it = m_resources.find(key);
        SP_ASSERT(it == m_resources.end() &&
            "Resource already loaded with different setup");

        auto resource = std::make_shared<T>();
        resource->Initialize(key);

        setupFn(*resource);

        if (!resource->Load(m_context))
            return nullptr;

        m_resources.emplace(key, resource);
        return resource;
    }

public:
    // =====================================================
    // Unload
    // =====================================================
    void Unload(const std::wstring& path);
    void UnloadAll();

private:
    std::wstring NormalizePath(const std::wstring& path) const;

private:
    static ResourceManager* s_instance;

    ResourceLoadContext m_context;
    std::unordered_map<std::wstring, std::shared_ptr<Resource>> m_resources;
    std::mutex m_mutex;
};
