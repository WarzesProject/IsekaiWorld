#pragma once

#include "IRenderDevice.h"

#if SE_ENABLE_DIRECT3D11

class D3D11RenderDevice final : public IRenderDevice
{
public:
	D3D11RenderDevice();
	~D3D11RenderDevice() final;

	bool Create(RenderConfig &config) final;
	void Destroy() final;

	bool BeginFrame() final;
	bool EndFrame() final;
};

#endif