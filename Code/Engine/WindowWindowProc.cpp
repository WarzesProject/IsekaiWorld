#include "stdafx.h"
#include "Window.h"
#include "Log.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (Window::IsValid())
	{
		static Window &window = GetSubsystem<Window>();

		switch (message)
		{
		case WM_ACTIVATEAPP: // Focus Change
			window.m_isActive = wParam != 0;
			if (window.m_isActive) Log::Debug("Window active");
			else Log::Debug("Window inactive");
			break;



		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(window, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
#endif