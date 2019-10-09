#pragma once

#include "IRenderer.h"
#include "D3DUtils.h"

#if SE_ENABLE_DIRECT3D11

class D3D11RenderDevice final : public IRenderer
{
public:
	D3D11RenderDevice();
	~D3D11RenderDevice() final;

	bool Create(RenderConfig &config) final;
	void Destroy() final;

	bool BeginFrame() final;
	bool EndFrame() final;

private:
	ComPtr<ID3D11Device> pd3d11Device;
	ComPtr<ID3D11DeviceContext> pd3d11Context;
};

#endif