#pragma once

#include "IRenderer.h"
#include "D3D11RenderDevice.h"

#if SE_DIRECT3D11

class D3D11Renderer final : public IRenderer
{
public:
	D3D11Renderer();
	~D3D11Renderer() final;

	bool Create(RenderConfig &config) final;
	void Destroy() final;

	bool BeginFrame() final;
	bool EndFrame() final;

private:
	D3D11RenderDevice m_device;
};

#endif