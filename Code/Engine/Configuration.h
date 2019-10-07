#pragma once

#include "LogConfig.h"
#include "WindowConfig.h"

struct Configuration
{
	LogConfig log;
	WindowConfig window;

	bool console = true;
};