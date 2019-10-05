#pragma once

#include "Module.h"

class Args : public Module<Args>
{
public:
	Args();
	~Args();

private:
	std::vector<std::string> m_args;

	struct ArgsPimpl *m_impl = nullptr;
};