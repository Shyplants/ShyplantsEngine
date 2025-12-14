#pragma once

#include <exception>
#include "Engine/Core/Log/LogMacros.h"

// Debug break abstraction
#if defined(_MSC_VER)
	#define SP_INTERNAL_DEBUGBREAK() __debugbreak()
#else
	#define SP_INTERNAL_DEBUGBREAK() std::abort()
#endif


// Fatal error
#if defined(_DEBUG)

	#define SP_FATAL(Message)								\
	do {													\
		SP_LOG(LogCore, ELogLevel::Fatal, Message);			\
		SP_INTERNAL_DEBUGBREAK();							\
	} while(0)

#else // Release

	#define SP_FATAL(Message)								\
	do {													\
		SP_LOG(LogCore, ELogLevel::Fatal, Message);			\
		std::terminate();									\
	} while(0)

#endif


// Assert (Debug only)
#if defined(_DEBUG)

	#define SP_ASSERT(Expr)														\
	do {																		\
		if (!(Expr))															\
		{																		\
			SP_LOG(LogCore, ELogLevel::Fatal, "Assertion failed: " #Expr);		\
			SP_INTERNAL_DEBUGBREAK();											\
		}																		\
	} while(0)
	
	#define SP_ASSERT_MSG(Expr, Message)										\
	do {																		\
		if (!(Expr))															\
		{																		\
			SP_LOG(LogCore, ELogLevel::Fatal, Message);							\
			SP_INTERNAL_DEBUGBREAK();											\
		}																		\
	} while(0)

#else // Release

	#define SP_ASSERT(Expr)					((void)0)
	#define SP_ASSERT_MSG(Expr, Message)	((void)0)

#endif


// Ensure (Logs error but does not break in Release)
#if defined(_DEBUG)

	#define SP_ENSURE(Expr)													\
	do {																	\
		if (!(Expr))														\
		{																	\
			SP_LOG(LogCore, ELogLevel::Error, "Ensure failed: " #Expr);		\
			SP_INTERNAL_DEBUGBREAK();										\
		}																	\
	} while(0)

#else // Release

	#define SP_ENSURE(Expr)													\
	do {																	\
		if (!(Expr))														\
		{																	\
			SP_LOG(LogCore, ELogLevel::Error, "Ensure failed: " #Expr);		\
		}																	\
	} while(0)

#endif