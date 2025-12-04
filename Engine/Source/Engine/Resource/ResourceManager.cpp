#include "Engine/Core/EnginePCH.h"
#include "Engine/Resource/ResourceManager.h"

ResourceManager* ResourceManager::Instance = nullptr;


void ResourceManager::Create()
{
	if (!Instance)
	{
		Instance = new ResourceManager;
	}
}

void ResourceManager::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

ResourceManager& ResourceManager::Get()
{
	assert(Instance && "ResourceManager::Create() was not called");
	return *Instance;
}

bool ResourceManager::Init(D3D11Renderer* renderer)
{
	m_renderer = renderer;

	return true;
}

void ResourceManager::Unload(const std::wstring& path)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_resources.find(path);
	if (it != m_resources.end())
	{
		it->second->Unload();
		m_resources.erase(it);
	}
}

void ResourceManager::UnloadAll()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	for (auto& pair : m_resources)
	{
		pair.second->Unload();
	}

	m_resources.clear();
}