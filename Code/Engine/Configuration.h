#pragma once

#include "RenderConfig.h"

struct Configuration
{
	LogConfig log;
	WindowConfig window;
	RenderConfig render;

	bool visibleConsole = true;
};