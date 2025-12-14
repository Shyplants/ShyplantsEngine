#pragma once

#include <fstream>
#include <mutex>
#include <string>

#include "Common/Types.h"

#include "Engine/Core/Log/LogLevel.h"
#include "Engine/Core/Log/LogCategory.h"

class LogSystem
{
public:
	static void Init(const std::wstring& logFilePath);
	static void Shutdown();

	static void Write(
		const LogCategory& category,
		ELogLevel level,
		const char* file,
		int32 line,
		const std::string& message
	);

	static void SetGlobalLevel(ELogLevel level);

private:
	static std::wofstream s_logFile;
	static std::mutex s_mutex;
	static ELogLevel s_globalLevel;


private:
	static std::wstring LevelToString(ELogLevel level);
};