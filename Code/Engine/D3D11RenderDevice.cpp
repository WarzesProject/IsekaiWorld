#include "stdafx.h"
#include "D3D11RenderDevice.h"
#include "Log.h"
#include "D3DUtils.h"
#if SE_ENABLE_DIRECT3D11
//-----------------------------------------------------------------------------
typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY)(REFIID _riid, void** _factory);
typedef HRESULT(WINAPI* PFN_CREATE_DXGI_FACTORY2)(UINT flags, REFIID _riid, void** _factory);
typedef HRESULT(WINAPI* PFN_GET_DXGI_DEBUG_INTERFACE)(UINT flags, REFIID _riid, void** _debug);
typedef HRESULT(WINAPI* PFN_GET_DXGI_DEBUG_INTERFACE1)(UINT flags, REFIID _riid, void** _debug);

struct
{
	bool available = false;
	HMODULE dxgiHandle = nullptr;
	HMODULE d3d11Handle = nullptr;
} _d3d11;
//-----------------------------------------------------------------------------
static PFN_CREATE_DXGI_FACTORY CreateDXGIFactory1 = nullptr;
static PFN_CREATE_DXGI_FACTORY2 CreateDXGIFactory2 = nullptr;
static PFN_GET_DXGI_DEBUG_INTERFACE DXGIGetDebugInterface = nullptr;
static PFN_GET_DXGI_DEBUG_INTERFACE1 DXGIGetDebugInterface1 = nullptr;
static PFN_D3D11_CREATE_DEVICE D3D11CreateDevice = nullptr;
//-----------------------------------------------------------------------------
#if SE_DEBUG
// Declare debug guids to avoid linking with "dxguid.lib"
static const GUID g_DXGI_DEBUG_ALL = { 0xe48ae283, 0xda80, 0x490b, {0x87, 0xe6, 0x43, 0xe9, 0xa9, 0xcf, 0xda, 0x8} };
static const GUID g_DXGI_DEBUG_DXGI = { 0x25cddaa4, 0xb1c6, 0x47e1, {0xac, 0x3e, 0x98, 0x87, 0x5b, 0x5a, 0x2e, 0x2a} };

// Check for SDK Layer support.
static inline bool D3D11SdkLayersAvailable()
{
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);

	return SUCCEEDED(hr);
}
#endif
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
	if (!isAvailable())
		return false;

#if SE_DEBUG
	bool enableValidationLayer = config.enableValidationLayer;
#else
	bool enableValidationLayer = false;
#endif

	if (!createFactory(enableValidationLayer))
		return false;

	// Get gpu adapter to use.
	ComPtr<IDXGIAdapter1> adapter;
	getHardwareAdapter(config.devicePreference, adapter.GetAddressOf());

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if SE_DEBUG
	if (enableValidationLayer && D3D11SdkLayersAvailable())
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL s_featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> d3dContext;
	HRESULT hr = E_FAIL;
	if (adapter)
	{
		hr = D3D11CreateDevice(
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			0,
			creationFlags,
			s_featureLevels,
			_countof(s_featureLevels),
			D3D11_SDK_VERSION,
			device.ReleaseAndGetAddressOf(),
			&m_d3dFeatureLevel,
			d3dContext.ReleaseAndGetAddressOf()
		);
	}

	if (FAILED(hr))
	{
		// If the initialization fails, fall back to the WARP device.
		// For more information on WARP, see:
		// http://go.microsoft.com/fwlink/?LinkId=286690
		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
			0,
			creationFlags,
			s_featureLevels,
			_countof(s_featureLevels),
			D3D11_SDK_VERSION,
			device.ReleaseAndGetAddressOf(),
			&m_d3dFeatureLevel,
			d3dContext.ReleaseAndGetAddressOf()
		);

		if (SUCCEEDED(hr))
		{
			OutputDebugStringA("Direct3D Adapter - WARP\n");
		}
	}
#endif

	ThrowIfFailed(hr);

#if SE_DEBUG
	if (enableValidationLayer)
	{
		ComPtr<ID3D11Debug> d3dDebug;
		if (SUCCEEDED(device.As(&d3dDebug)))
		{
			ComPtr<ID3D11InfoQueue> d3dInfoQueue;
			if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
			{
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
					// TODO: Add more message IDs here as needed.
				};
				D3D11_INFO_QUEUE_FILTER filter = {};
				filter.DenyList.NumIDs = _countof(hide);
				filter.DenyList.pIDList = hide;
				d3dInfoQueue->AddStorageFilterEntries(&filter);
			}
		}
	}
#endif

	// Obtain Direct3D 11.1 interfaces (if available)
	ThrowIfFailed(device.As(&m_d3dDevice));
	ThrowIfFailed(d3dContext.As(&m_d3dContext));

	// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and ensures that the application will only render after each VSync, minimizing power consumption.
	ComPtr<IDXGIDevice3> dxgiDevice;
	if (SUCCEEDED(m_d3dDevice.As(&dxgiDevice)))
		ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));

	....

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
bool D3D11RenderDevice::isAvailable()
{
	if (_d3d11.available)
		return true;

	_d3d11.dxgiHandle = LoadLibrary(L"dxgi.dll");
	if (_d3d11.dxgiHandle == nullptr)
	{
		Log::Error("DXGI.dll not found");
		return false;
	}

	CreateDXGIFactory2 = (PFN_CREATE_DXGI_FACTORY2)GetProcAddress(_d3d11.dxgiHandle, "CreateDXGIFactory2");
	CreateDXGIFactory1 = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(_d3d11.dxgiHandle, "CreateDXGIFactory1");
	if (CreateDXGIFactory1 == nullptr)
	{
		CreateDXGIFactory1 = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(_d3d11.dxgiHandle, "CreateDXGIFactory");
	}

	DXGIGetDebugInterface1 = (PFN_GET_DXGI_DEBUG_INTERFACE1)GetProcAddress(_d3d11.dxgiHandle, "DXGIGetDebugInterface1");
	DXGIGetDebugInterface = (PFN_GET_DXGI_DEBUG_INTERFACE)GetProcAddress(_d3d11.dxgiHandle, "DXGIGetDebugInterface");

	_d3d11.d3d11Handle = LoadLibraryW(L"d3d11.dll");
	if (_d3d11.d3d11Handle == nullptr)
	{
		Log::Error("d3d11.dll not found");
		return false;
	}

	D3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)GetProcAddress(_d3d11.d3d11Handle, "D3D11CreateDevice");
	if (D3D11CreateDevice == nullptr)
	{
		Log::Error("D3D11CreateDevice symbol not found in d3d11.dll!");
		return false;
	}

	_d3d11.available = true;
	return true;
}
//-----------------------------------------------------------------------------
bool D3D11RenderDevice::createFactory(bool enableValidationLayer)
{
#if defined(_DEBUG)
	bool debugDXGI = false;
	if (enableValidationLayer)
	{
		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			debugDXGI = true;

			ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

			dxgiInfoQueue->SetBreakOnSeverity(g_DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(g_DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

			DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
			{
				80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
			};
			DXGI_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			dxgiInfoQueue->AddStorageFilterEntries(g_DXGI_DEBUG_DXGI, &filter);
		}
	}

	if (!debugDXGI)
#endif
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
	}

	m_tearingSupported = false;
	ComPtr<IDXGIFactory5> factory5;
	if (SUCCEEDED(m_dxgiFactory.As(&factory5)))
	{
		BOOL allowTearing = FALSE;
		HRESULT tearingSupportHR = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));

		if (SUCCEEDED(tearingSupportHR) && allowTearing)
			m_tearingSupported = true;
	}
	
	return true;
}
//-----------------------------------------------------------------------------
void D3D11RenderDevice::getHardwareAdapter(GpuDevicePreference powerPreference, IDXGIAdapter1 **ppAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;

#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
	ComPtr<IDXGIFactory6> factory6;
	HRESULT hr = m_dxgiFactory.As(&factory6);
	if (SUCCEEDED(hr))
	{
		DXGI_GPU_PREFERENCE dxgiGpuPreference = DXGI_GPU_PREFERENCE_UNSPECIFIED;
		switch (powerPreference)
		{
		case GpuDevicePreference::LowPower:
			dxgiGpuPreference = DXGI_GPU_PREFERENCE_MINIMUM_POWER;
			break;
		case GpuDevicePreference::HighPerformance:
			dxgiGpuPreference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
			break;
		default:
			break;
		}

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(adapterIndex, dxgiGpuPreference, IID_PPV_ARGS(&adapter)); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			break;
		}
	}
#endif
	if (!adapter)
	{
		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			break;
		}
	}

	*ppAdapter = adapter.Detach();
}
//-----------------------------------------------------------------------------
#endif