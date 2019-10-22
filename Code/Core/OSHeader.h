#pragma once

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