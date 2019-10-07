#include "stdafx.h"
#include "Window.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (Window::IsValid())
	{
		static Window &window = GetSubsystem<Window>();

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProcW(window, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
#endif