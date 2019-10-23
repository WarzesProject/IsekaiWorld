#include "stdafx.h"
#include "Platform.h"
//-----------------------------------------------------------------------------
OSPlatform::OSPlatform()
{
#if SE_COMPILER_MSVC
//	HRESULT hr;
//	if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
//	{
//		CriticalErrorExit("Failed to initialize COM");
//		return;
//	}
#endif

	setValid(true);
}
//-----------------------------------------------------------------------------
void OSPlatform::PrintDebugOutput(std::string_view str)
{
#if SE_COMPILER_MSVC && SE_DEBUG
	OutputDebugStringA(str.data());
	OutputDebugStringA("\n");
#else
	(void)str;
#endif
}
//-----------------------------------------------------------------------------