#pragma once

#include "RenderConfig.h"

class SE_NO_VTABLE IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool Create(RenderConfig &config) = 0;
	virtual void Destroy() = 0;
	
	virtual bool BeginFrame() = 0;
	virtual bool EndFrame() = 0;
};