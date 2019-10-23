#pragma once

class Platform : public Subsystem<Platform>
{
public:
	Platform();
	~Platform();

	void PrintDebugOutput(std::string_view str);
};