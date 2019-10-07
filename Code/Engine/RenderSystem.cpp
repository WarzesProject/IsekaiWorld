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
	}

	Log::Info(text);
}
//-----------------------------------------------------------------------------
RenderSystem::RenderSystem(RenderConfig &config)
	: m_config(config)
{
	if (config.gapi == RenderBackend::Direct3D11)
		m_device = new D3D11RenderDevice();

	logRenderBackend(config.gapi);


	setValid(true);
}
//-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
	SafeDelete(m_device);
}
//-----------------------------------------------------------------------------