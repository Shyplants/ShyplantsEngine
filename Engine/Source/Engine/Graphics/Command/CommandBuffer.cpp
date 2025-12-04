#include "Engine/Core/EnginePCH.h"
#include "Engine/Graphics/Command/CommandBuffer.h"
#include "Engine/Graphics/Command/DrawCommand.h"


void CommandBuffer::Submit(const DrawCommand& cmd)
{
	std::scoped_lock lock(m_mutex);
	m_commands.push_back(cmd);
}

std::vector<DrawCommand>& CommandBuffer::AccessCommands()
{
	std::scoped_lock lock(m_mutex);
	return m_commands;
}

void CommandBuffer::Clear()
{
	std::scoped_lock lock(m_mutex);
	m_commands.clear();
}


