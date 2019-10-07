#include "stdafx.h"
#include "Application.h"
#include "Args.h"
#include "Console.h"
#include "Log.h"
#include "OSPlatformUtils.h"
#include "Window.h"
#include "Input.h"
#include "RenderSystem.h"
//-----------------------------------------------------------------------------
struct AppPimpl
{
	Configuration config;
	bool activeUpdate = true;
	bool activeBeginFrame = true;
	bool activeEndFrame = true;
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
bool Application::init(const Configuration &config)
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
	
	if (config.visibleConsole)
		SE_INIT_SUBSYSTEM(Console::Create());

	SE_INIT_SUBSYSTEM(Log::Create(config.log));
	SE_INIT_SUBSYSTEM(OSPlatformUtils::Create());
	SE_INIT_SUBSYSTEM(Window::Create(config.window));
	SE_INIT_SUBSYSTEM(Input::Create());
	SE_INIT_SUBSYSTEM(RenderSystem::Create(config.render));

#undef SE_INIT_SUBSYSTEM

	return !isError;
}
//-----------------------------------------------------------------------------
bool Application::beginFrame()
{
	if (IsErrorCriticalExit()) return false;
	if (!m_impl->activeBeginFrame) return true;

	static auto &render = GetSubsystem<RenderSystem>();

	if (!render.BeginFrame())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::endFrame()
{
	if (IsErrorCriticalExit()) return false;
	if (!m_impl->activeEndFrame) return true;

	static auto &render = GetSubsystem<RenderSystem>();

	if (!render.EndFrame())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::update()
{
	if (IsErrorCriticalExit()) return false;

	static Window &window = GetSubsystem<Window>();

	TODO("при неактивности окна или его сворачивании нужно снижать потребление ресурсов. но с неактивностью пока косяк - ведь оно может быть неактивно, но все еще видимо - а значит всеже должно рендерится... возможно лучше просто понижать vsync? код пока оставлен для примера обработки данного события");
	TODO(" а вообще при деактивации окна должна освобождаться мышь (если она захвачена, а при активации возвращаться к норме");
	if (window.IsActive())
	{
		m_impl->activeBeginFrame = true;
		m_impl->activeEndFrame = true;
		m_impl->activeUpdate = true;
	}
	else
	{
		m_impl->activeBeginFrame = false;
		m_impl->activeEndFrame = false;
	}

	if (!m_impl->activeUpdate) return true;

	if (!window.Update())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
	RenderSystem::Destroy();
	Input::Destroy();
	Window::Destroy();
	OSPlatformUtils::Destroy();
	Log::Destroy();
	Console::Destroy();
	Args::Destroy();
}
//-----------------------------------------------------------------------------