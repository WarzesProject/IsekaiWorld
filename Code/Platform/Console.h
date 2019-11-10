#pragma once

class Console : public Subsystem<Console>
{
public:
	Console(bool enable);

	void Print(std::string_view str);

private:
};