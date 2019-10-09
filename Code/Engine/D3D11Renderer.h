#pragma once

#include "IRenderer.h"
#include "D3DUtils.h"

#if SE_ENABLE_DIRECT3D11

class D3D11Renderer final : public IRenderer
{
public:
	D3D11Renderer();
	~D3D11Renderer() final;

	bool Create(RenderConfig &config) final;
	void Destroy() final;

	bool BeginFrame() final;
	bool EndFrame() final;
};

#endif