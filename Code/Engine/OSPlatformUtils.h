#pragma once

#include "Subsystem.h"

class OSPlatformUtils : public Subsystem<OSPlatformUtils>
{
public:
	OSPlatformUtils();

	void PrintDebugOutput(std::string_view str);
	
	void SetCurrentThreadName(std::string_view name);
};