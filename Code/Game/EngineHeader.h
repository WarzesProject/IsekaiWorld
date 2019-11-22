#pragma once

//-----------------------------------------------------------------------------
// Core Header
#include <Core/BaseHeader.h>
#include <Core/LogConfig.h>
#include <Core/Error.h>

//-----------------------------------------------------------------------------
// Platform Header
#include <Platform/WindowConfig.h>

//-----------------------------------------------------------------------------
// Math Header

//-----------------------------------------------------------------------------
// Engine Header
#include <Engine/Application.h>

//-----------------------------------------------------------------------------
// link lib
#if SE_COMPILER_MSVC
#	pragma comment(lib, "Core.lib")
#	pragma comment(lib, "Math.lib")
#	pragma comment(lib, "Platform.lib")
#	pragma comment(lib, "Engine.lib")
#	pragma comment(lib, "3rdparty.lib")
#	pragma comment(lib, "NewCode.lib")

#	pragma comment(lib, "winmm.lib")
#	pragma comment(lib, "Ws2_32.lib")
#	pragma comment(lib, "shlwapi.lib")

#	pragma comment(lib, "OpenGL32.lib")

#	pragma comment(lib, "d3d11.lib")
#	pragma comment(lib, "dxgi.lib")
#	pragma comment(lib, "dxguid.lib")
#	pragma comment(lib, "dsound.lib")
//#	pragma comment(lib, "Xaudio2.lib")
#	pragma comment(lib, "dinput8.lib")
#	pragma comment(lib, "xinput9_1_0.lib")
#endif