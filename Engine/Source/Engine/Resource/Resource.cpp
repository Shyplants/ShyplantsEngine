#include "Engine/Core/EnginePCH.h"
#include "Engine/Resource/Resource.h"

Resource::Resource(ResourceType type)
	: m_type(type)
{
}

Resource::~Resource() = default;