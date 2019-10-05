#pragma once

#include "PlatformCompiler.h"
#include "PlatformOS.h"

#if defined(_WIN32_WINNT) || defined(WINVER)
#	error "_WIN32_WINNT / WINVER"
#endif

// Platform OS Define
#if SE_PLATFORM_WINDOWS
#	define _WIN32_WINNT 0x0601
#	define WINVER _WIN32_WINNT
#	define WIN32_LEAN_AND_MEAN
#	define WIN32_EXTRA_LEAN
#	define NOMINMAX
#endif