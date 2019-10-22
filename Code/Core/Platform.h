#pragma once

#include "PlatformCompiler.h"
#include "PlatformOS.h"
#if SE_PLATFORM_WINDOWS
#	include "PlatormWin32.h"
#endif

// GAPI
#if SE_ENABLE_DIRECT3D11 || SE_ENABLE_DIRECT3D12
#	define SE_ENABLE_DIRECT3D 1
#else
#	define SE_ENABLE_DIRECT3D 0
#endif