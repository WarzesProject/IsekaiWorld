#include "stdafx.h"
#include "Window.h"
#include "Error.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
constexpr LPCWSTR WINDOW_CLASS_NAME = L"SapphireWindow";
#endif
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
#endif
//-----------------------------------------------------------------------------
Window::Window(WindowConfig &config)
	: m_config(config)
{
	if (!initDPI())
		return;

#if SE_PLATFORM_WINDOWS
	m_native.instance = GetModuleHandle(nullptr);
	if (!m_native.instance)
	{
		CriticalErrorExit("Failed to get module handle");
		return;
	}

	// Register class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = windowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_native.instance;
	wc.hIcon = LoadIcon(m_native.instance, MAKEINTRESOURCEW(101));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = nullptr;
	m_windowClass = RegisterClassEx(&wc);
	if (!m_windowClass)
	{
		CriticalErrorExit("Failed to register window class");
		return;
	}

	// Set Style types
	m_windowWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_BORDER | WS_DLGFRAME | WS_THICKFRAME | WS_GROUP | WS_TABSTOP;
	if (m_config.resizable)
		m_windowWindowedStyle |= WS_SIZEBOX | WS_MAXIMIZEBOX;
	m_windowStyle = m_windowWindowedStyle;
	m_windowFullscreenStyle = WS_CLIPSIBLINGS | WS_GROUP | WS_TABSTOP;	
	m_windowExStyle = WS_EX_APPWINDOW;

	// Size
	RECT windowRect = { 0, 0, static_cast<LONG>(m_config.width), static_cast<LONG>(m_config.height) };
	if (!AdjustWindowRectEx(&windowRect, m_windowStyle, FALSE, m_windowExStyle))
	{
		CriticalErrorExit("Failed to adjust window rectangle");
		return;
	}
	const int width = (m_config.width > 0) ? windowRect.right - windowRect.left : CW_USEDEFAULT;
	const int height = (m_config.height > 0) ? windowRect.bottom - windowRect.top : CW_USEDEFAULT;

	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	// Title
	const int titleBufferSize = MultiByteToWideChar(CP_UTF8, 0, m_config.title.c_str(), -1, nullptr, 0);
	if (titleBufferSize == 0)
	{
		CriticalErrorExit("Failed to convert UTF-8 to wide char");
		return;
	}
	std::vector<WCHAR> titleBuffer(titleBufferSize);
	if (MultiByteToWideChar(CP_UTF8, 0, m_config.title.c_str(), -1, titleBuffer.data(), titleBufferSize) == 0)
	{
		CriticalErrorExit("Failed to convert UTF-8 to wide char");
		return;
	}

	m_native.hwnd = CreateWindowEx(m_windowExStyle, WINDOW_CLASS_NAME, titleBuffer.data(), m_windowStyle, x, y, width, height, nullptr, nullptr, m_native.instance, nullptr);
	if (!m_native.hwnd)
	{
		CriticalErrorExit("Failed to create window");
		return;
	}

	m_monitor = MonitorFromWindow(m_native.hwnd, MONITOR_DEFAULTTONEAREST);

	if (m_config.fullscreen)
		switchFullscreen(m_config.fullscreen);

	if (!GetClientRect(m_native.hwnd, &windowRect))
	{
		CriticalErrorExit("Failed to get client rectangle");
		return;
	}

	m_config.width = static_cast<uint32_t>(windowRect.right - windowRect.left);
	m_config.height = static_cast<uint32_t>(windowRect.bottom - windowRect.top);

	//if (!RegisterTouchWindow(m_native.hwnd, 0))
	//	Log("Failed to enable touch for window");

	ShowWindow(m_native.hwnd, SW_SHOW);

	SetLastError(ERROR_SUCCESS);

#endif

	setValid(true);
}
//-----------------------------------------------------------------------------
Window::~Window()
{
#if SE_PLATFORM_WINDOWS
	if (m_native.hwnd)
		DestroyWindow(m_native.hwnd);

	if (m_windowClass)
		UnregisterClass(WINDOW_CLASS_NAME, m_native.instance);
#endif
}
//-----------------------------------------------------------------------------
bool Window::Update()
{
#if SE_PLATFORM_WINDOWS
	if (PeekMessage(&m_message, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_message);
		DispatchMessage(&m_message);
		if (m_message.message == WM_QUIT)
			return false;
	}
#endif
	return true;
}
//-----------------------------------------------------------------------------
const WindowNativeData& Window::GetNative() const
{
	return m_native;
}
//-----------------------------------------------------------------------------
void Window::switchFullscreen(bool newFullscreen)
{
#if SE_PLATFORM_WINDOWS
	if (m_config.exclusiveFullscreen)
	{
		//d3d11::RenderDevice->setFullscreen(newFullscreen);
	}
	else
	{
		m_windowStyle = (newFullscreen ? m_windowFullscreenStyle : m_windowWindowedStyle) | WS_VISIBLE;
		if (!SetWindowLong(m_native.hwnd, GWL_STYLE, m_windowStyle))
			throw std::system_error(GetLastError(), std::system_category(), "Failed to set window style");

		if (newFullscreen)
		{
			RECT windowRect;
			if (!GetWindowRect(m_native.hwnd, &windowRect))
			{
				CriticalErrorExit("Failed to get window rectangle");
				return;
			}

			m_windowX = windowRect.left;
			m_windowY = windowRect.top;
			m_windowWidth = windowRect.right - windowRect.left;
			m_windowHeight = windowRect.bottom - windowRect.top;

			MONITORINFO info;
			info.cbSize = sizeof(MONITORINFO);
			if (!GetMonitorInfo(m_monitor, &info))
			{
				CriticalErrorExit("Failed to get monitor info");
				return;
			}

			if (!SetWindowPos(m_native.hwnd, nullptr,
				info.rcMonitor.left, info.rcMonitor.top,
				info.rcMonitor.right - info.rcMonitor.left,
				info.rcMonitor.bottom - info.rcMonitor.top,
				SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER))
			{
				CriticalErrorExit("Failed to set window position");
				return;
			}
		}
		else
		{
			if (!SetWindowPos(m_native.hwnd, nullptr, m_windowX, m_windowY, m_windowWidth, m_windowHeight, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER))
			{
				CriticalErrorExit("Failed to set window position");
				return;
			}
		}
	}
#endif
}
//-----------------------------------------------------------------------------