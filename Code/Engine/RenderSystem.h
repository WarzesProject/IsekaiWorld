#pragma once

#include "RenderConfig.h"

class IRenderer;

class RenderSystem : public Subsystem<RenderSystem>
{
public:
	RenderSystem(RenderConfig &config);
	~RenderSystem();

	bool BeginFrame();
	bool EndFrame();

	IRenderer* GetRenderer() const { return m_renderer; }

private:
	RenderConfig &m_config;
	IRenderer *m_renderer = nullptr;
};