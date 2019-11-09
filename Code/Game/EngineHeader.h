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
#	pragma comment(lib, "winmm.lib")
//#	pragma comment(lib, "imm32.lib")
//#	pragma comment(lib, "version.lib")
//#	pragma comment(lib, "OpenGL32.lib")
//#	pragma comment(lib, "Glu32.lib")
//#	pragma comment(lib, "d3dcompiler.lib")
//#	pragma comment(lib, "d3d11.lib")
//#	pragma comment(lib, "dxgi.lib")
//#	pragma comment(lib, "dxguid.lib")
#endif