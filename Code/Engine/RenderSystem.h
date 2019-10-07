#pragma once

#include "Subsystem.h"
#include "RenderConfig.h"

class IRenderDevice;

class RenderSystem : public Subsystem<RenderSystem>
{
public:
	RenderSystem(RenderConfig &config);
	~RenderSystem();

	bool BeginFrame();
	bool EndFrame();


private:
	RenderConfig &m_config;

	IRenderDevice *m_device = nullptr;
};