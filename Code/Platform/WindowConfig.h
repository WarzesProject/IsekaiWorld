#pragma once

struct WindowConfig
{
	std::string title = "Engine";
	uint16_t width = 640;
	uint16_t height = 480;
	bool highDpi = true;
	bool resizable = true;
	bool fullscreen = false;
};