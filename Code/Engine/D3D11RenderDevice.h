#pragma once

#include "RenderConfig.h"

#if SE_DIRECT3D11

class D3D11RenderDevice
{
public:
	bool Init(RenderConfig &config);
	void Close();

private:
};

#endif