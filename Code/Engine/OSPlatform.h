#pragma once

#include "Subsystem.h"

class OSPlatform : public Subsystem<OSPlatform>
{
public:
	OSPlatform();

	void PrintDebugOutput(std::string_view str);
	
	void SetCurrentThreadName(std::string_view name);
};