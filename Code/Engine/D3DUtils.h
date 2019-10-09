#pragma once

#include "VideoAdapter.h"

#if SE_ENABLE_DIRECT3D11

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

void DXThrowIfFailed(const HRESULT hr, const char *info);
void DXThrowIfCreateFailed(const HRESULT hr, const char *interfaceName, const char *contextInfo = nullptr);

std::string GetVendorByID(unsigned short id);

VideoAdapterDescriptor DXGetVideoAdapterDesc(IDXGIAdapter *adapter);

// Returns the list of all supported Direct3D feature levels.
std::vector<D3D_FEATURE_LEVEL> DXGetFeatureLevels(D3D_FEATURE_LEVEL maxFeatureLevel);

#endif