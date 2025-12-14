#include "Engine/Core/Log/LogSystem.h"

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

std::wofstream LogSystem::s_logFile;
std::mutex LogSystem::s_mutex;
ELogLevel LogSystem::s_globalLevel = ELogLevel::Trace;

void LogSystem::Init(const std::wstring& logFilePath)
{
	std::lock_guard<std::mutex> lock(s_mutex);

	std::filesystem::create_directories(
		std::filesystem::path(logFilePath).parent_path());

	s_logFile.open(logFilePath, std::ios::out | std::ios::trunc);

	if (s_logFile.is_open())
	{
		s_logFile << L"==== ShyplantsEngine Log Started ====\n";
	}
}

void LogSystem::Shutdown()
{
	std::lock_guard<std::mutex> lock(s_mutex);

	if (s_logFile.is_open())
	{
		s_logFile << L"==== Log Shutdown ====\n";
		s_logFile.close();
	}
}

void LogSystem::Write(const LogCategory& category, ELogLevel level, const char* file, int32 line, const std::string& message)
{
	if (level < s_globalLevel)
		return;

	std::lock_guard<std::mutex> lock(s_mutex);

	if (!s_logFile.is_open())
		return;

	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	std::tm localTime{};
#ifdef _WIN32
	localtime_s(&localTime, &time);
#else
	localtime_r(&time, &localTime);
#endif

	s_logFile
		<< L"[" << std::put_time(&localTime, L"%H:%M:%S") << L"]"
		<< L"[" << category.Name << "]"
		<< L"[" << LevelToString(level) << L"] "
		<< std::wstring(message.begin(), message.end()) << L" "
		<< L"(" << file << L":" << line << L")\n";

	s_logFile.flush();

	if (level == ELogLevel::Fatal)
	{
		__debugbreak();
	}
}

void LogSystem::SetGlobalLevel(ELogLevel level)
{
	s_globalLevel = level;
}

std::wstring LogSystem::LevelToString(ELogLevel level)
{
	switch (level)
	{
	case ELogLevel::Trace:		return L"Trace";
	case ELogLevel::Info:		return L"Info";
	case ELogLevel::Warning:	return L"Warning";
	case ELogLevel::Error:		return L"Error";
	case ELogLevel::Fatal:		return L"Fatal";
	default:					return L"UnKnown";
	}
}
