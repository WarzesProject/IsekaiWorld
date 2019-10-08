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

	IRenderDevice* GetRenderDevice() const { return m_device; }

private:
	RenderConfig &m_config;

	IRenderDevice *m_device = nullptr;
};