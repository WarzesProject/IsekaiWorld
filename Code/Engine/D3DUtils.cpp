#include "stdafx.h"
#include "D3DUtils.h"
#include "StringUtils.h"
#include "Error.h"
#include "D3DVendor.h"

#if SE_ENABLE_DIRECT3D11
//-----------------------------------------------------------------------------
static const char* DXErrorToStr(const HRESULT hr)
{
#define SE_CASE_TO_STR(VALUE) case VALUE: return #VALUE
	
	switch (hr)
	{
		// see https://msdn.microsoft.com/en-us/library/windows/desktop/aa378137(v=vs.85).aspx
		SE_CASE_TO_STR(S_OK);
		SE_CASE_TO_STR(S_FALSE);
		SE_CASE_TO_STR(E_ABORT);
		SE_CASE_TO_STR(E_ACCESSDENIED);
		SE_CASE_TO_STR(E_FAIL);
		SE_CASE_TO_STR(E_HANDLE);
		SE_CASE_TO_STR(E_INVALIDARG);
		SE_CASE_TO_STR(E_NOINTERFACE);
		SE_CASE_TO_STR(E_NOTIMPL);
		SE_CASE_TO_STR(E_OUTOFMEMORY);
		SE_CASE_TO_STR(E_POINTER);
		SE_CASE_TO_STR(E_UNEXPECTED);

		// see https://msdn.microsoft.com/en-us/library/windows/desktop/bb509553(v=vs.85).aspx
		SE_CASE_TO_STR(DXGI_ERROR_DEVICE_HUNG);
		SE_CASE_TO_STR(DXGI_ERROR_DEVICE_REMOVED);
		SE_CASE_TO_STR(DXGI_ERROR_DEVICE_RESET);
		SE_CASE_TO_STR(DXGI_ERROR_DRIVER_INTERNAL_ERROR);
		SE_CASE_TO_STR(DXGI_ERROR_FRAME_STATISTICS_DISJOINT);
		SE_CASE_TO_STR(DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE);
		SE_CASE_TO_STR(DXGI_ERROR_INVALID_CALL);
		SE_CASE_TO_STR(DXGI_ERROR_MORE_DATA);
		SE_CASE_TO_STR(DXGI_ERROR_NONEXCLUSIVE);
		SE_CASE_TO_STR(DXGI_ERROR_NOT_CURRENTLY_AVAILABLE);
		SE_CASE_TO_STR(DXGI_ERROR_NOT_FOUND);
		SE_CASE_TO_STR(DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED);
		SE_CASE_TO_STR(DXGI_ERROR_REMOTE_OUTOFMEMORY);
		SE_CASE_TO_STR(DXGI_ERROR_WAS_STILL_DRAWING);
		SE_CASE_TO_STR(DXGI_ERROR_UNSUPPORTED);
		SE_CASE_TO_STR(DXGI_ERROR_ACCESS_LOST);
		SE_CASE_TO_STR(DXGI_ERROR_WAIT_TIMEOUT);
		SE_CASE_TO_STR(DXGI_ERROR_SESSION_DISCONNECTED);
		SE_CASE_TO_STR(DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE);
		SE_CASE_TO_STR(DXGI_ERROR_CANNOT_PROTECT_CONTENT);
		SE_CASE_TO_STR(DXGI_ERROR_ACCESS_DENIED);
		SE_CASE_TO_STR(DXGI_ERROR_NAME_ALREADY_EXISTS);
		SE_CASE_TO_STR(DXGI_ERROR_SDK_COMPONENT_MISSING);

		// see https://msdn.microsoft.com/en-us/library/windows/desktop/ff476174(v=vs.85).aspx
		SE_CASE_TO_STR(D3D10_ERROR_FILE_NOT_FOUND);
		SE_CASE_TO_STR(D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);
		SE_CASE_TO_STR(D3D11_ERROR_FILE_NOT_FOUND);
		SE_CASE_TO_STR(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);
		SE_CASE_TO_STR(D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS);
		SE_CASE_TO_STR(D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD);

#if SE_ENABLE_DIRECT3D12
		SE_CASE_TO_STR(D3D12_ERROR_ADAPTER_NOT_FOUND);
		SE_CASE_TO_STR(D3D12_ERROR_DRIVER_VERSION_MISMATCH);
#endif
	}

#undef SE_CASE_TO_STR
	return nullptr;
}
//-----------------------------------------------------------------------------
[[noreturn]] static void DXThrowFailure(const HRESULT hr, const char *info)
{
	std::string s;

	if (info)
	{
		s += info;
		s += " (error code = ";
	}
	else
		s += "Direct3D operation failed (error code = ";

	if (auto err = DXErrorToStr(hr))
		s += err;
	else
	{
		s += "0x";
		s += ToHex(hr);
	}

	s += ")";

	CriticalErrorExit(s);
}
//-----------------------------------------------------------------------------
void DXThrowIfFailed(const HRESULT hr, const char *info)
{
	if (FAILED(hr))
		DXThrowFailure(hr, info);
}
//-----------------------------------------------------------------------------
void DXThrowIfCreateFailed(const HRESULT hr, const char *interfaceName, const char *contextInfo)
{
	if (FAILED(hr))
	{
		std::string s;
		{
			s = "failed to create instance of <";
			s += interfaceName;
			s += '>';
			if (contextInfo != nullptr)
			{
				s += ' ';
				s += contextInfo;
			}
		}
		DXThrowFailure(hr, s.c_str());
	}
}
//-----------------------------------------------------------------------------
VideoAdapterDescriptor DXGetVideoAdapterDesc(IDXGIAdapter *adapter)
{
	ComPtr<IDXGIOutput> output;

	/* Query adapter description */
	DXGI_ADAPTER_DESC desc;
	adapter->GetDesc(&desc);

	/* Setup adapter information */
	VideoAdapterDescriptor videoAdapterDesc;

	videoAdapterDesc.name = std::wstring(desc.Description);
	videoAdapterDesc.vendor = GetVendorByID(desc.VendorId);
	videoAdapterDesc.videoMemory = static_cast<uint64_t>(desc.DedicatedVideoMemory);

	/* Enumerate over all adapter outputs */
	for (UINT j = 0; adapter->EnumOutputs(j, &output) != DXGI_ERROR_NOT_FOUND; ++j)
	{
		/* Get output description */
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		/* Query number of display modes */
		UINT numModes = 0;
		output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);

		/* Query display modes */
		std::vector<DXGI_MODE_DESC> modeDesc(numModes);

		auto hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modeDesc.data());
		DXThrowIfFailed(hr, "failed to get display mode list with format DXGI_FORMAT_R8G8B8A8_UNORM");

		/* Add output information to the current adapter */
		VideoOutputDescriptor videoOutput;

		for (UINT i = 0; i < numModes; ++i)
		{
			DisplayModeDescriptor displayMode;
			{
				displayMode.resolution.width = modeDesc[i].Width;
				displayMode.resolution.height = modeDesc[i].Height;
				displayMode.refreshRate = (modeDesc[i].RefreshRate.Denominator > 0 ? modeDesc[i].RefreshRate.Numerator / modeDesc[i].RefreshRate.Denominator : 0);
			}
			videoOutput.displayModes.push_back(displayMode);
		}

		/* Remove duplicate display modes */
		std::sort(videoOutput.displayModes.begin(), videoOutput.displayModes.end(), CompareSWO);

		videoOutput.displayModes.erase(
			std::unique(videoOutput.displayModes.begin(), videoOutput.displayModes.end()),
			videoOutput.displayModes.end()
		);

		/* Add output to the list and release handle */
		videoAdapterDesc.outputs.push_back(videoOutput);

		output.Reset();
	}

	return videoAdapterDesc;
}
//-----------------------------------------------------------------------------
std::vector<D3D_FEATURE_LEVEL> DXGetFeatureLevels(D3D_FEATURE_LEVEL maxFeatureLevel)
{
	std::vector<D3D_FEATURE_LEVEL> featureLevels =
	{
#if SE_ENABLE_DIRECT3D12
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
#endif
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	auto it = std::remove_if(featureLevels.begin(), featureLevels.end(),
		[maxFeatureLevel](D3D_FEATURE_LEVEL entry)
		{
			return (entry > maxFeatureLevel);
		}
	);
	featureLevels.erase(it, featureLevels.end());

	return featureLevels;
}
//-----------------------------------------------------------------------------
#endif