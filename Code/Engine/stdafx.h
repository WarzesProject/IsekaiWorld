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
#include <functional>
#include <memory>

//-----------------------------------------------------------------------------
// Platform Header
#if SE_COMPILER_MSVC
#	if SE_DEBUG
#		include <crtdbg.h>
#	endif

#	include <Windows.h>
#	include <shellapi.h>

#	if SE_ENABLE_DIRECT3D11 || SE_ENABLE_DIRECT3D12
#		include <wrl/client.h>
#	endif
#endif

#if SE_ENABLE_DIRECT3D11
#	define D3D11_NO_HELPERS
#	include <d3d11_1.h>
#	//include <d3dcommon.h>
#	//include <dxgiformat.h>
#if defined(NTDDI_WIN10_RS2)
#   include <dxgi1_6.h>
#else
#   include <dxgi1_5.h>
#endif
#	if SE_DEBUG
#		include <dxgidebug.h>
#	endif
#endif

//-----------------------------------------------------------------------------
// 3rd Header

//-----------------------------------------------------------------------------
// Other Engine Header
#include "D3D11Core.h" // TODO: удалить

#pragma warning(pop)