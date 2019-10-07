#pragma once

#include "Subsystem.h"

class Args : public Subsystem<Args>
{
public:
	Args();
	~Args();

private:
	std::vector<std::string> m_args;

	struct ArgsPimpl *m_impl = nullptr;
};