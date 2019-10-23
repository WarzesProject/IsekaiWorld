#include "stdafx.h"
#include "Window.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;

	if (Window::IsValid())
	{
		static Window &window = GetSubsystem<Window>();
		handled = window.onWindowMessage(message, wParam, lParam);
	}
	return handled ? 0 : DefWindowProc(hwnd, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
bool Window::onWindowMessage(unsigned msg, unsigned wParam, unsigned lParam)
{
	bool handled = false;
	static Window &window = GetSubsystem<Window>();

	switch (msg)
	{
	case WM_ACTIVATEAPP: // Focus Change
		window.m_isActive = wParam != 0;
		if (window.m_isActive) Log::Debug("Window active");
		else Log::Debug("Window inactive");
		handled = true;
		break;

	case WM_SIZE:
		window.resize();
		handled = true;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		handled = true;
		break;
	}

	return handled;
}
//-----------------------------------------------------------------------------
#endif