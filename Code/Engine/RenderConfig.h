#pragma once

enum class RenderBackend
{
	Direct3D11
};

enum class GpuDevicePreference
{
	/// No GPU preference.
	DontCare,
	/// Prefer integrated GPU.
	LowPower,
	/// Prefer high performance/discrete GPU.
	HighPerformance,
};

struct RenderConfig
{
	RenderBackend gapi = RenderBackend::Direct3D11;
	GpuDevicePreference devicePreference = GpuDevicePreference::HighPerformance;

#if SE_DEBUG && SE_ENABLE_DIRECT3D11
	bool enableValidationLayer = true;
#endif
};