#pragma once

#include "RenderConfig.h"
#include "D3DUtils.h"

#if SE_ENABLE_DIRECT3D11

class D3D11RenderDevice
{
public:
	bool Init(RenderConfig &config);
	void Close();

private:
	bool createFactory();
	void queryVideoAdapters();
	void createDevice();
	bool createDeviceWithFlags(IDXGIAdapter *adapter, const std::vector<D3D_FEATURE_LEVEL> &featureLevels, UINT flags, HRESULT &hr);

	ComPtr<IDXGIFactory> m_factory;
	ComPtr<ID3D11Device> m_device;
#if SE_D3D11_ENABLE_FEATURELEVEL >= 1
	ComPtr<ID3D11Device1> m_device1;
#endif
#if SE_D3D11_ENABLE_FEATURELEVEL >= 2
	ComPtr<ID3D11Device2> m_device2;
#endif
#if SE_D3D11_ENABLE_FEATURELEVEL >= 3
	ComPtr<ID3D11Device3> m_device3;
#endif
	ComPtr<ID3D11DeviceContext> m_context;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_10_0;

	std::vector<VideoAdapterDescriptor> m_videoAdapterDescs;
};

#endif