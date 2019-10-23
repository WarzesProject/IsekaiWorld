#pragma once

//-----------------------------------------------------------------------------
// Config Engine
#include "Version.h"
#include "EngineConfig.h"

//-----------------------------------------------------------------------------
// Base Header
#include "DetectCompiler.h"
#include "DetectOS.h"
#if SE_PLATFORM_WINDOWS
#	include "ConfigPlatformWin32.h"
#endif

// GAPI
#if SE_ENABLE_DIRECT3D11 || SE_ENABLE_DIRECT3D12
#	define SE_ENABLE_DIRECT3D 1
#else
#	define SE_ENABLE_DIRECT3D 0
#endif

#include "Macros.h"
#include "InlineFunc.h"

//-----------------------------------------------------------------------------
// 3rd-party Header
#pragma warning(push, 1)
#include "STDHeader.h"
#include "OSHeader.h"
#include "GAPIHeader.h"
#pragma warning(pop)