#pragma once

class OSPlatform : public Subsystem<OSPlatform>
{
public:
	OSPlatform();

	void PrintDebugOutput(std::string_view str);
	
	void SetCurrentThreadName(std::string_view name);
};