#include "stdafx.h"
#include "Application.h"
#include "Args.h"
#include "Console.h"
#include "Log.h"
#include "OSPlatformUtils.h"
#include "Window.h"
#include "Input.h"
//-----------------------------------------------------------------------------
struct AppPimpl
{
	ApplicationConfig config;
};
//-----------------------------------------------------------------------------
Application::Application()
	: m_impl(new AppPimpl)
{
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	SafeDelete(m_impl);
}
//-----------------------------------------------------------------------------
bool Application::init(const ApplicationConfig &config)
{
	m_impl->config = config;

	if (!initSubsystem())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::initSubsystem()
{
	bool isError = false;
	auto &config = m_impl->config;

#define SE_INIT_SUBSYSTEM(_ss) ((isError) = ((isError) || (IsErrorCriticalExit()) || !(_ss)))

	SE_INIT_SUBSYSTEM(Args::Create());
	
	if (config.console)
		SE_INIT_SUBSYSTEM(Console::Create());

	SE_INIT_SUBSYSTEM(Log::Create(config.log));
	SE_INIT_SUBSYSTEM(OSPlatformUtils::Create());
	SE_INIT_SUBSYSTEM(Window::Create(config.window));
	SE_INIT_SUBSYSTEM(Input::Create());

#undef SE_INIT_SUBSYSTEM

	return !isError;
}
//-----------------------------------------------------------------------------
bool Application::beginFrame()
{
	if (IsErrorCriticalExit()) return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::endFrame()
{
	if (IsErrorCriticalExit()) return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::update()
{
	if (IsErrorCriticalExit()) return false;

	static Window &currentWnd = GetSubsystem<Window>();

	if (!currentWnd.Update())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
	Input::Destroy();
	Window::Destroy();
	OSPlatformUtils::Destroy();
	Log::Destroy();
	Console::Destroy();
	Args::Destroy();
}
//-----------------------------------------------------------------------------