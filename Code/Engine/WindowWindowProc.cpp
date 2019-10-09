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
		//case WM_PAINT:
		//{
		//	PAINTSTRUCT ps;
		//	BeginPaint(window.m_native.hwnd, &ps);
		//	EndPaint(window.m_native.hwnd, &ps);
		//	return 0;
		//}

		case WM_GETMINMAXINFO:
		{
			// установка минимально-допустимого размера окна
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 320;
			lpMMI->ptMinTrackSize.y = 240;
			return 0;
		}

		case WM_ACTIVATEAPP: // Focus Change
			window.m_isActive = wParam != 0;
			if (window.m_isActive) Log::Debug("Window active");
			else Log::Debug("Window inactive");
			return 0;

		case WM_SIZE:
			window.resize();
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(window, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
#endif