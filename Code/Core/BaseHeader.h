#pragma once

//-----------------------------------------------------------------------------
#include "Version.h"
#include "EngineConfig.h"
#include "DetectCompiler.h"
#include "DetectOS.h"
#include "ConfigPlatformWin32.h"
#include "PostConfigEngine.h"
#include "Macros.h"
#include "InlineFunc.h"

//-----------------------------------------------------------------------------
// 3rd-party Header
#if SE_COMPILER_MSVC
#	pragma warning(push, 1)
#endif
#include "STDHeader.h"
#include "OSHeader.h"
#if SE_COMPILER_MSVC
#	pragma warning(pop)
#endif

//-----------------------------------------------------------------------------
#include "BaseTypes.h"