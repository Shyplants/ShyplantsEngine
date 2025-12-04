#pragma once

#include <vector>
#include <mutex>

struct DrawCommand;

class CommandBuffer
{
public:
	void Submit(const DrawCommand& cmd);
	std::vector<DrawCommand>& AccessCommands();

	void Clear();

private:
	std::vector<DrawCommand> m_commands;
	std::mutex m_mutex;
};