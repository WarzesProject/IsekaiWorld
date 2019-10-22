#pragma once

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