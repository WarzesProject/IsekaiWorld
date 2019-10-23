#include "stdafx.h"
#include "Platform.h"
//-----------------------------------------------------------------------------
Platform::Platform()
{
#if SE_PLATFORM_WINDOWS
	if (timeBeginPeriod(1) == TIMERR_NOCANDO)
	{
		TODO("Log");//"Unable to set timer resolution to 1ms.This can cause significant waste in performance for waiting threads.";
	}
#endif
	setValid(true);
}
//-----------------------------------------------------------------------------
Platform::~Platform()
{
#if SE_PLATFORM_WINDOWS
	timeEndPeriod(1);
#endif
}
//-----------------------------------------------------------------------------
void Platform::PrintDebugOutput(std::string_view str)
{
#if SE_COMPILER_MSVC && SE_DEBUG
	OutputDebugStringA(str.data());
	OutputDebugStringA("\n");
#else
	(void)str;
#endif
}
//-----------------------------------------------------------------------------