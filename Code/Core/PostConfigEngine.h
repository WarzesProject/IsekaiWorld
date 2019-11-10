#pragma once

#if !SE_PLATFORM_WINDOWS
#	undef SE_ENABLE_DIRECT3D11
#	define SE_ENABLE_DIRECT3D11 0
#	undef SE_ENABLE_DIRECT3D12
#	define SE_ENABLE_DIRECT3D12 0
#endif

#if SE_ENABLE_DIRECT3D11 || SE_ENABLE_DIRECT3D12
#	define SE_ENABLE_DIRECT3D 1
#else
#	define SE_ENABLE_DIRECT3D 0
#endif