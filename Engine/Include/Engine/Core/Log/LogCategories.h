#pragma once

#include "Engine/Core/Log/LogCategory.h"

inline LogCategory LogCore{ "Core",   ELogLevel::Info };
inline LogCategory LogRender{ "Render", ELogLevel::Info };
inline LogCategory LogIO{ "IO",     ELogLevel::Info };
inline LogCategory LogNet{ "Net",    ELogLevel::Info };
inline LogCategory LogGame{ "Game",   ELogLevel::Info };