#pragma once

#include "Subsystem.h"
#include "WindowConfig.h"
#include "WindowNativeData.h"

class Window : public Subsystem<Window>
{
public:
	Window(WindowConfig &config);
	~Window();

	bool Update();

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
};