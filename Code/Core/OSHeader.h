#pragma once

#if SE_COMPILER_MSVC
#	if SE_DEBUG
#		include <crtdbg.h>
#	endif

#	include <Windows.h>
#	include <shellapi.h>
#	include <strsafe.h>
#	include <TimeAPI.h>

#	if SE_DIRECT3D
#		include <wrl/client.h>
#	endif
#endif