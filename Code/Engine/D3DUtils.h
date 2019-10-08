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

inline std::string D3DFeatureLevelToVersion(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_12_1:    return "12.1";
	case D3D_FEATURE_LEVEL_12_0:    return "12.0";
	case D3D_FEATURE_LEVEL_11_1:    return "11.1";
	case D3D_FEATURE_LEVEL_11_0:    return "11.0";
	case D3D_FEATURE_LEVEL_10_1:    return "10.1";
	case D3D_FEATURE_LEVEL_10_0:    return "10.0";
	case D3D_FEATURE_LEVEL_9_3:     return "9.3";
	case D3D_FEATURE_LEVEL_9_2:     return "9.2";
	case D3D_FEATURE_LEVEL_9_1:     return "9.1";
	}

	return "";
}

inline static const char* GetVendorByID(unsigned id)
{
	switch (id)
	{
	case 0x1002: return "Advanced Micro Devices, Inc.";
	case 0x10de: return "NVIDIA Corporation";
	case 0x102b: return "Matrox Electronic Systems Ltd.";
	case 0x1414: return "Microsoft Corporation";
	case 0x5333: return "S3 Graphics Co., Ltd.";
	case 0x8086: return "Intel Corporation";
	case 0x80ee: return "Oracle Corporation";
	case 0x15ad: return "VMware Inc.";
	}
	return "";
}

#endif