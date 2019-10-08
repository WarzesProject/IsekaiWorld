#pragma once

// GraphicsDevice information .
struct GraphicsDeviceInfo
{
	/// Rendering API name.
	std::string backendName;

	/// The hardware gpu device vendor name.
	std::string vendorName;

	/// The hardware gpu device vendor id.
	uint32_t vendorId;

	/// The hardware gpu device name.
	std::string deviceName;
};

/// Describes features supported by given instance of GpuDevice.
struct GraphicsDeviceFeatures
{
	bool    independentBlend = false;
	bool    computeShader = false;
	bool    geometryShader = false;
	bool    tessellationShader = false;
	bool    sampleRateShading = false;
	bool    dualSrcBlend = false;
	bool    logicOp = false;
	bool    multiViewport = false;
	bool    indexUInt32 = false;
	bool    drawIndirect = false;
	bool    alphaToOne = false;
	bool    fillModeNonSolid = false;
	bool    samplerAnisotropy = false;
	bool    textureCompressionBC = false;
	bool    textureCompressionPVRTC = false;
	bool    textureCompressionETC2 = false;
	bool    textureCompressionATC = false;
	bool    textureCompressionASTC = false;
	bool    pipelineStatisticsQuery = false;
	/// Specifies whether 1D textures are supported.
	bool    texture1D = false;
	/// Specifies whether 3D textures are supported.
	bool    texture3D = false;
	/// Specifies whether 2D array textures are supported.
	bool    texture2DArray = false;
	/// Specifies whether cube array textures are supported.
	bool    textureCubeArray = false;
	/// Specifies whether raytracing is supported.
	bool    raytracing = false;
};

struct GraphicsDeviceLimits
{
	uint32_t        maxTextureDimension1D;
	uint32_t        maxTextureDimension2D;
	uint32_t        maxTextureDimension3D;
	uint32_t        maxTextureDimensionCube;
	uint32_t        maxTextureArrayLayers;
	uint32_t        maxColorAttachments;
	uint32_t        maxUniformBufferSize;
	uint64_t        minUniformBufferOffsetAlignment;
	uint32_t        maxStorageBufferSize;
	uint64_t        minStorageBufferOffsetAlignment;
	float           maxSamplerAnisotropy;
	uint32_t        maxViewports;
	uint32_t        maxViewportDimensions[2];
	uint32_t        maxTessellationPatchSize;
	float           pointSizeRange[2];
	float           lineWidthRange[2];
	uint32_t        maxComputeSharedMemorySize;
	uint32_t        maxComputeWorkGroupCount[3];
	uint32_t        maxComputeWorkGroupInvocations;
	uint32_t        maxComputeWorkGroupSize[3];
};

/// Describes caps of graphics device.
struct GraphicsDeviceCapabilities
{
	/// Specifies all supported hardware features.
	GraphicsDeviceFeatures          features;

	/// Specifies all rendering limitations.
	GraphicsDeviceLimits            limits;
};