#pragma once

class Console : public Subsystem<Console>
{
public:
	Console();

	void Print(std::string_view str);

private:
};