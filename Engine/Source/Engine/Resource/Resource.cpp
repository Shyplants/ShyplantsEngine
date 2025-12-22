#include "Engine/PCH/EnginePCH.h"

#include "Engine/Resource/Resource.h"
#include <filesystem>

Resource::Resource(ResourceType type)
    : m_type(type)
{
}

Resource::~Resource() = default;

bool Resource::Initialize(const std::wstring& path)
{
    m_path = path;
    m_name = std::filesystem::path(path).filename().wstring();
    return true;
}
