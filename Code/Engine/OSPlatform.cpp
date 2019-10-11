#include "stdafx.h"
#include "OSPlatform.h"
#include "Error.h"
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
void OSPlatform::SetCurrentThreadName(std::string_view name)
{
//#if SE_COMPILER_MSVC
//	constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;
//#  pragma pack(push,8)
//	typedef struct tagTHREADNAME_INFO
//	{
//		DWORD dwType; // Must be 0x1000.
//		LPCSTR szName; // Pointer to name (in user addr space).
//		DWORD dwThreadID; // Thread ID (-1=caller thread).
//		DWORD dwFlags; // Reserved for future use, must be zero.
//	} THREADNAME_INFO;
//#  pragma pack(pop)
//
//	THREADNAME_INFO info;
//	info.dwType = 0x1000;
//	info.szName = name.data();
//	info.dwThreadID = static_cast<DWORD>(-1);
//	info.dwFlags = 0;
//
//	__try
//	{
//		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
//	}
//	__except (EXCEPTION_EXECUTE_HANDLER)
//	{
//	}
//#endif
}
//-----------------------------------------------------------------------------