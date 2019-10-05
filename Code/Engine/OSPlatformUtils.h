#pragma once

#include "Module.h"

class OSPlatformUtils : public Module<OSPlatformUtils>
{
public:
	OSPlatformUtils();

	void PrintDebugOutput(std::string_view str);
	
	void SetCurrentThreadName(std::string_view name);
};