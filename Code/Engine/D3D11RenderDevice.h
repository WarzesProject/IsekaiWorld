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
};

#endif