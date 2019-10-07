#pragma once

#include "Subsystem.h"
#include "WindowConfig.h"
#include "WindowNativeData.h"

class Window : public Subsystem<Window>
{
#if SE_PLATFORM_WINDOWS
	friend LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
#endif
public:
	Window(WindowConfig &config);
	~Window();

	bool Update();

	bool IsActive() const { return m_isActive; }
	uint16_t GetWidth() const { return m_config.width; }
	uint16_t GetHeight() const { return m_config.height; }

	const WindowNativeData& GetNative() const;
	
private:
	void switchFullscreen(bool newFullscreen);
	bool initDPI();

	WindowConfig &m_config;
	WindowNativeData m_native;

#if SE_PLATFORM_WINDOWS
	ATOM m_windowClass = 0;	
	DWORD m_windowStyle = 0;
	DWORD m_windowExStyle = 0;
	DWORD m_windowWindowedStyle = 0;
	DWORD m_windowFullscreenStyle = 0;
	HMONITOR m_monitor = nullptr;
	MSG m_message;
#endif

	int m_windowX = 0;
	int m_windowY = 0;
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	bool m_isActive = true;
};