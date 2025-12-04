#pragma once

#include <string>
#include "Engine/Resource/ResourceType.h"

class Resource
{
public:
	explicit Resource(ResourceType type);
	virtual ~Resource();

	Resource(const Resource&) = delete;
	Resource& operator=(const Resource&) = delete;

public:
	virtual bool Load(const std::wstring& path) = 0;
	virtual void Unload() = 0;

public:
	ResourceType GetType() const { return m_type; }
	const std::wstring& GetPath() const { return m_path; }
	const std::wstring& GetName() const { return m_name; }
	
	void SetPath(const std::wstring& path) { m_path = path; }
	void SetName(const std::wstring& name) { m_name = name; }

protected:
	ResourceType m_type{ ResourceType::UnKnown };

	std::wstring m_path;
	std::wstring m_name;
	
};