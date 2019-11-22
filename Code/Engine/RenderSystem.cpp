#include "stdafx.h"
#include "RenderSystem.h"
#include "D3D11Renderer.h"
//-----------------------------------------------------------------------------
inline void logRenderBackend(RenderBackend backend)
{
	std::string text;

	switch (backend)
	{
	case RenderBackend::Direct3D11:
		text = "Render Backend: Direct3D 11";
		break;
	}

	Log::Info(text);
}
//-----------------------------------------------------------------------------
RenderSystem::RenderSystem(RenderConfig &config)
	: m_config(config)
{
#if SE_DIRECT3D11
	if (m_config.gapi == RenderBackend::Direct3D11)
		m_renderer = new D3D11Renderer();
#endif

	if (!m_renderer || !m_renderer->Create(m_config))
		return;

	logRenderBackend(m_config.gapi);
	
	setValid(true);
}
//-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
	SafeDelete(m_renderer);
}
//-----------------------------------------------------------------------------
bool RenderSystem::BeginFrame()
{
	if (!m_renderer->BeginFrame())
		return false;
	return true;
}
//-----------------------------------------------------------------------------
bool RenderSystem::EndFrame()
{
	if (!m_renderer->EndFrame())
		return false;
	return true;
}
//-----------------------------------------------------------------------------