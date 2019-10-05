#pragma once

#pragma warning(push, 1)

//-----------------------------------------------------------------------------
// Base Engine Header
#include "Engine/BaseHeader.h"

//-----------------------------------------------------------------------------
// STL Header
#include <string>
#include <string_view>
#include <vector>

//-----------------------------------------------------------------------------
// Platform Header
#if SE_COMPILER_MSVC
#	if SE_DEBUG
#		include <crtdbg.h>
#	endif
#endif

//-----------------------------------------------------------------------------
// 3rd Header

//-----------------------------------------------------------------------------
// Other Engine Header
#include "Engine/Application.h"

#pragma warning(pop)

//-----------------------------------------------------------------------------
#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "3rdparty.lib")
//#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "imm32.lib")
//#pragma comment(lib, "version.lib")
//#pragma comment(lib, "OpenGL32.lib")
//#pragma comment(lib, "Glu32.lib")


//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxguid.lib")
