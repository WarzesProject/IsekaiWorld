#pragma once

#include "Subsystem.h"

class Console : public Subsystem<Console>
{
public:
	Console();

	void Print(std::string_view str);

private:
};