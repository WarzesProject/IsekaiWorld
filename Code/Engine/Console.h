#pragma once

#include "Module.h"

class Console : public Module<Console>
{
public:
	Console();

	void Print(std::string_view str);
};