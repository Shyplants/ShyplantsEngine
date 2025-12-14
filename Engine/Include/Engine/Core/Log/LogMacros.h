#pragma once

#include "Engine/Core/Log/LogSystem.h"
#include <sstream>

#define SP_LOG(Category, Level, Message)	\
do {										\
	std::ostringstream oss;					\
	oss << Message;							\
	LogSystem::Write(						\
		Category,							\
		Level,								\
		__FILE__,							\
		__LINE__,							\
		oss.str());							\
} while(0)									\