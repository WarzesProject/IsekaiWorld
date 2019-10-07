#pragma once

#include "RenderConfig.h"

SE_NO_VTABLE class IRenderDevice
{
public:
	virtual ~IRenderDevice() = default;

	virtual bool Create(RenderConfig &config) = 0;
	virtual void Destroy() = 0;

	virtual bool BeginFrame() = 0;
	virtual bool EndFrame() = 0;
};