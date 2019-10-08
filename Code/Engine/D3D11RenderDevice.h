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

private:
	bool isAvailable();
	bool createFactory(bool enableValidationLayer);
	void getHardwareAdapter(GpuDevicePreference powerPreference, IDXGIAdapter1 **ppAdapter);
	void initializeInfoAndCaps(IDXGIAdapter1 *adapter);

	ComPtr<IDXGIFactory2> m_dxgiFactory;
	D3D_FEATURE_LEVEL m_d3dFeatureLevel = D3D_FEATURE_LEVEL_10_0;
	ComPtr<ID3D11Device1> m_d3dDevice;
	ComPtr<ID3D11DeviceContext1> m_d3dContext;

	// Whether tearing is supported.
	bool m_tearingSupported = false;
};

#endif