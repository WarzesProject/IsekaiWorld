#pragma once

#include "RenderConfig.h"
#include "RenderTypes.h"

SE_NO_VTABLE class IRenderDevice
{
public:
	virtual ~IRenderDevice() = default;

	virtual bool Create(RenderConfig &config) = 0;
	virtual void Destroy() = 0;
	
	virtual bool BeginFrame() = 0;
	virtual bool EndFrame() = 0;

	const RenderDeviceInfo& GetInfo() const
	{
		return m_info;
	}
	const RenderDeviceCapabilities& GetCaps() const
	{
		return m_caps;
	}

protected:
	RenderDeviceInfo m_info = {};
	RenderDeviceCapabilities m_caps = {};
};