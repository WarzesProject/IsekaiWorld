#pragma once

#if SE_ENABLE_DIRECT3D11

#include "Error.h"

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch DirectX API errors
		static char s_str[64] = {};
		sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(hr));
		CriticalErrorExit(s_str);
	}
}

#endif