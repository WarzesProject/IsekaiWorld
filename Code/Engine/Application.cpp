#include "stdafx.h"
#include "Application.h"
#include "Args.h"
#include "Console.h"
#include "Log.h"
#include "OSPlatformUtils.h"
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

	bool isError = false;

	isError = (isError || !Args::Create());
	isError = (isError || !Console::Create());
	isError = (isError || !Log::Create(m_impl->config.log));
	isError = (isError || !OSPlatformUtils::Create());

	GetModule<Log>().Print(LogType::Info, "hell\nhe");
	GetModule<Log>().Print(LogType::Info, "hell\nhe");

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

	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
	OSPlatformUtils::Destroy();
	Log::Destroy();
	Console::Destroy();
	Args::Destroy();
}
//-----------------------------------------------------------------------------