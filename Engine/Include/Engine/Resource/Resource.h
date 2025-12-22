#pragma once

#include <string>
#include "Engine/Resource/ResourceType.h"

struct ResourceLoadContext;

/*
    Resource
    -------------------------------------------------
    - 모든 엔진 리소스의 베이스 클래스
    - 외부 시스템에 직접 의존하지 않음
*/
class Resource
{
public:
    explicit Resource(ResourceType type);
    virtual ~Resource();

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

public:
    // =====================================================
    // Lifecycle
    // =====================================================
    bool Initialize(const std::wstring& path);

    virtual bool Load(const ResourceLoadContext& context) = 0;
    virtual void Unload() = 0;

public:
    // =====================================================
    // Accessors
    // =====================================================
    ResourceType GetType() const { return m_type; }
    const std::wstring& GetPath() const { return m_path; }
    const std::wstring& GetName() const { return m_name; }
    bool IsLoaded() const { return m_loaded; }

protected:
    void SetLoaded(bool loaded) { m_loaded = loaded; }

private:
    ResourceType m_type{ ResourceType::UnKnown };
    std::wstring m_path;
    std::wstring m_name;
    bool m_loaded{ false };
};
