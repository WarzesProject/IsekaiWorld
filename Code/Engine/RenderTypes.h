#pragma once

static constexpr uint32_t MaxVertexAttributes = 16u;
static constexpr uint32_t MaxVertexBufferBindings = 8u;
static constexpr uint32_t MaxColorAttachments = 8u;

struct RenderDeviceInfo
{
	std::string backendName;
	std::string vendorName;
	uint32_t vendorId; 
	std::string deviceName;
};

struct GraphicsDeviceFeatures
{
	bool independentBlend = false;
	bool computeShader = false;
	bool geometryShader = false;
	bool tessellationShader = false;
	bool sampleRateShading = false;
	bool dualSrcBlend = false;
	bool logicOp = false;
	bool multiViewport = false;
	bool indexUInt32 = false;
	bool drawIndirect = false;
	bool alphaToOne = false;
	bool fillModeNonSolid = false;
	bool samplerAnisotropy = false;
	bool textureCompressionBC = false;
	bool textureCompressionPVRTC = false;
	bool textureCompressionETC2 = false;
	bool textureCompressionATC = false;
	bool textureCompressionASTC = false;
	bool pipelineStatisticsQuery = false;
	bool texture1D = false;
	bool texture3D = false;
	bool texture2DArray = false;
	bool textureCubeArray = false;
	bool raytracing = false;
};

struct RenderDeviceLimits
{
	uint32_t maxTextureDimension1D;
	uint32_t maxTextureDimension2D;
	uint32_t maxTextureDimension3D;
	uint32_t maxTextureDimensionCube;
	uint32_t maxTextureArrayLayers;
	uint32_t maxColorAttachments;
	uint32_t maxUniformBufferSize;
	uint64_t minUniformBufferOffsetAlignment;
	uint32_t maxStorageBufferSize;
	uint64_t minStorageBufferOffsetAlignment;
	float maxSamplerAnisotropy;
	uint32_t maxViewports;
	uint32_t maxViewportDimensions[2];
	uint32_t maxTessellationPatchSize;
	float pointSizeRange[2];
	float lineWidthRange[2];
	uint32_t maxComputeSharedMemorySize;
	uint32_t maxComputeWorkGroupCount[3];
	uint32_t maxComputeWorkGroupInvocations;
	uint32_t maxComputeWorkGroupSize[3];
};

struct RenderDeviceCapabilities
{
	GraphicsDeviceFeatures features;
	RenderDeviceLimits limits;
};