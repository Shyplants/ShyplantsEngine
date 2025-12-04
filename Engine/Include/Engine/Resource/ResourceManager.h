#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

#include "Engine/Resource/Resource.h"
#include "Engine/Resource/Texture/Texture.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"

class D3D11Renderer;

class ResourceManager
{
public:
public:
	static void Create();
	static void Destroy();
	static ResourceManager& Get();

	bool Init(D3D11Renderer* renderer);

private:
	static ResourceManager* Instance;

public:
	template<class T>
	std::shared_ptr<T> Load(const std::wstring path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		auto it = m_resources.find(path);
		if (it != m_resources.end())
		{
			// 이미 로드된 리소스 반환
			return std::dynamic_pointer_cast<T>(it->second);
		}

		// 리소스 생성
		std::shared_ptr<T> resource;
		if constexpr (std::is_same_v<T, Texture>)
		{
			resource = std::make_shared<T>(m_renderer);
		}
		else
		{
			resource = std::make_shared<T>();
		}
		

		if (!resource->Load(path))
			return nullptr;

		resource->SetPath(path);
		m_resources[path] = resource;

		return resource;
	}

	void Unload(const std::wstring& path);
	void UnloadAll();

private:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

private:
	D3D11Renderer* m_renderer{ nullptr };

	std::unordered_map<std::wstring, std::shared_ptr<Resource>> m_resources;
	std::mutex m_mutex;

};