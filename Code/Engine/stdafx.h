#pragma once

#pragma warning(push, 1)

//-----------------------------------------------------------------------------
// Base Engine Header
#include "BaseHeader.h"

//-----------------------------------------------------------------------------
// STL Header
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

//-----------------------------------------------------------------------------
// Platform Header
#if SE_COMPILER_MSVC
#	if SE_DEBUG
#		include <crtdbg.h>
#	endif

#	include <Windows.h>
#	include <shellapi.h>
#endif

//-----------------------------------------------------------------------------
// 3rd Header

//-----------------------------------------------------------------------------
// Other Engine Header

#pragma warning(pop)