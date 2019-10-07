#pragma once

#include "LogConfig.h"
#include "WindowConfig.h"
#include "RenderConfig.h"

struct Configuration
{
	LogConfig log;
	WindowConfig window;
	RenderConfig render;

	bool visibleConsole = true;
};