#include "stdafx.h"
#include "D3D11RenderDevice.h"
#include "Log.h"
#include "D3DUtils.h"

#if SE_ENABLE_DIRECT3D11
//-----------------------------------------------------------------------------
D3D11Renderer::D3D11Renderer()
{

}
//-----------------------------------------------------------------------------
D3D11Renderer::~D3D11Renderer()
{
	Destroy();
}
//-----------------------------------------------------------------------------
bool D3D11Renderer::Create(RenderConfig &config)
{	
	UINT creationFlags = 0;

#if SE_DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
#if SE_PLATFORM_WINRT
		D3D_FEATURE_LEVEL_11_1,
#endif
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	ComPtr<ID3D11Device> pd3d11Device;
	ComPtr<ID3D11DeviceContext> pd3d11Context;	
	D3D_FEATURE_LEVEL d3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pd3d11Device, &d3dFeatureLevel, &pd3d11Context);
	if (FAILED(hr))
	{
		Log::Error("Failed to create D3D11 native device and immediate context");
		return false;
	}



	return true;
}
//-----------------------------------------------------------------------------
void D3D11Renderer::Destroy()
{

}
//-----------------------------------------------------------------------------
bool D3D11Renderer::BeginFrame()
{
	return true;
}
//-----------------------------------------------------------------------------
bool D3D11Renderer::EndFrame()
{
	return true;
}
//-----------------------------------------------------------------------------
#endif