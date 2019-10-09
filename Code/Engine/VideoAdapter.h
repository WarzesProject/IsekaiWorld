#pragma once

#include "DisplayFlags.h"

struct VideoOutputDescriptor
{
	std::vector<DisplayModeDescriptor> displayModes;
};

struct VideoAdapterDescriptor
{
	std::wstring name;
	std::string vendor;
	uint64_t videoMemory = 0;
	std::vector<VideoOutputDescriptor>  outputs;
};