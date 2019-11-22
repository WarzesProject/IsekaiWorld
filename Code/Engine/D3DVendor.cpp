#include "stdafx.h"
#include "D3DVendor.h"

#if SE_DIRECT3D11
//-----------------------------------------------------------------------------
// используется только в DX. В OGL своя реализация. А в вулкане?
std::string GetVendorByID(unsigned short id)
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
//-----------------------------------------------------------------------------

#endif