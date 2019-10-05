#pragma once

struct WindowNativeData
{
#if SE_PLATFORM_WINDOWS
	HINSTANCE instance = nullptr;
	HWND hwnd = nullptr;
#endif
};