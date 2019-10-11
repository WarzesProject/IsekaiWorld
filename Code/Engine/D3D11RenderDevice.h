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
	void createStateManagerAndCommandQueue();

	ComPtr<IDXGIFactory> m_factory;
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_10_0;

	std::vector<VideoAdapterDescriptor> m_videoAdapterDescs;
};

#endif