#include "stdafx.h"
#include "RenderSystem.h"
#include "D3D11RenderDevice.h"
#include "Log.h"
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
#if SE_ENABLE_DIRECT3D11
	if (m_config.gapi == RenderBackend::Direct3D11)
		m_device = new D3D11RenderDevice();
#endif

	if (!m_device || !m_device->Create(m_config))
		return;

	logRenderBackend(m_config.gapi);


	setValid(true);
}
//-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
	SafeDelete(m_device);
}
//-----------------------------------------------------------------------------
bool RenderSystem::BeginFrame()
{
	if (!m_device->BeginFrame())
		return false;
	return true;
}
//-----------------------------------------------------------------------------
bool RenderSystem::EndFrame()
{
	if (!m_device->EndFrame())
		return false;
	return true;
}
//-----------------------------------------------------------------------------