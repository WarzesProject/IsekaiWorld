#include "stdafx.h"
#include "D3D11RenderDevice.h"

#if SE_ENABLE_DIRECT3D11
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::Init(RenderConfig &config)
{
	if (!createFactory())
		return false;

	TODO("сделать проверки на ошибки более точными");
	queryVideoAdapters();

	createDevice();

	return false;
}
//-----------------------------------------------------------------------------
void D3D11RenderDevice::Close()
{
}
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::createFactory()
{
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&m_factory));
	DXThrowIfCreateFailed(hr, "IDXGIFactory");

	return true;
}
//-----------------------------------------------------------------------------
void D3D11RenderDevice::queryVideoAdapters()
{
	ComPtr<IDXGIAdapter> adapter;

	for (UINT i = 0; m_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		m_videoAdapterDescs.push_back(DXGetVideoAdapterDesc(adapter.Get()));
		adapter.Reset();
	}
}
//-----------------------------------------------------------------------------
void D3D11RenderDevice::createDevice()
{
	/* Find list of feature levels to select from, and statically determine maximal feature level */
	auto featureLevels = DXGetFeatureLevels(
#if SE_D3D11_ENABLE_FEATURELEVEL >= 1
		D3D_FEATURE_LEVEL_11_1
#else
		D3D_FEATURE_LEVEL_11_0
#endif
	);

	HRESULT hr = 0;

#if SE_DEBUG
	if (!createDeviceWithFlags(nullptr, featureLevels, D3D11_CREATE_DEVICE_DEBUG, hr))
		createDeviceWithFlags(nullptr, featureLevels, 0, hr);
#else
	createDeviceWithFlags(nullptr, featureLevels, 0, hr);
#endif

	DXThrowIfCreateFailed(hr, "ID3D11Device");

	/* Try to get an extended D3D11 device */
#if SE_D3D11_ENABLE_FEATURELEVEL >= 3
	hr = device_->QueryInterface(IID_PPV_ARGS(&device3_));
	if (FAILED(hr))
#endif
	{
#if SE_D3D11_ENABLE_FEATURELEVEL >= 2
		hr = device_->QueryInterface(IID_PPV_ARGS(&device2_));
		if (FAILED(hr))
#endif
		{
#if SE_D3D11_ENABLE_FEATURELEVEL >= 1
			device_->QueryInterface(IID_PPV_ARGS(&device1_));
#endif
		}
	}
}
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::createDeviceWithFlags(IDXGIAdapter *adapter, const std::vector<D3D_FEATURE_LEVEL> &featureLevels, UINT flags, HRESULT &hr)
{
	for (D3D_DRIVER_TYPE driver : { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE })
	{
		hr = D3D11CreateDevice(
			adapter,                                    // Video adapter
			driver,                                     // Driver type
			0,                                          // Software rasterizer module (none)
			flags,                                      // Flags
			featureLevels.data(),                       // Feature level
			static_cast<UINT>(featureLevels.size()),    // Num feature levels
			D3D11_SDK_VERSION,                          // SDK version
			m_device.ReleaseAndGetAddressOf(),          // Output device
			&m_featureLevel,                            // Output feature level
			m_context.ReleaseAndGetAddressOf()          // Output device context
		);
		if (SUCCEEDED(hr))
			return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
#endif