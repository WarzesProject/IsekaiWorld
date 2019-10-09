#include "stdafx.h"
#include "D3D11RenderDevice.h"
#include "Log.h"
#include "D3DUtils.h"

#if SE_ENABLE_DIRECT3D11
//-----------------------------------------------------------------------------
D3D11RenderDevice::D3D11RenderDevice()
{

}
//-----------------------------------------------------------------------------
D3D11RenderDevice::~D3D11RenderDevice()
{
	Destroy();
}
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::Create(RenderConfig &config)
{	

	return true;
}
//-----------------------------------------------------------------------------
void D3D11RenderDevice::Destroy()
{

}
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::BeginFrame()
{
	return true;
}
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::EndFrame()
{
	return true;
}
//-----------------------------------------------------------------------------
#endif