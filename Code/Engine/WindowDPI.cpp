#include "stdafx.h"
#include "Window.h"
//-----------------------------------------------------------------------------
bool Window::initDPI()
{
	if (m_config.highDpi)
	{
#if SE_PLATFORM_WINDOWS
		HMODULE shcoreModule = LoadLibraryW(L"shcore.dll");
		if (shcoreModule)
		{
			using SetProcessDpiAwarenessProc = HRESULT(STDAPICALLTYPE *)(int value);
			SetProcessDpiAwarenessProc setProcessDpiAwarenessProc = reinterpret_cast<SetProcessDpiAwarenessProc>(GetProcAddress(shcoreModule, "SetProcessDpiAwareness"));

			constexpr int PROCESS_PER_MONITOR_DPI_AWARE = 2;
			if (setProcessDpiAwarenessProc)
				setProcessDpiAwarenessProc(PROCESS_PER_MONITOR_DPI_AWARE);
		}
#endif
	}

	return true;
}
//-----------------------------------------------------------------------------