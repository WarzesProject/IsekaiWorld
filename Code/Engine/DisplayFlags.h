#pragma once

struct DisplayModeDescriptor
{
	// Display resolution (in pixels).
	uint32_t width = 0; 
	uint32_t height = 0;

	// Display refresh rate (in Hz).
	uint32_t refreshRate = 0;
};

bool operator==(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs);
bool operator!=(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs);

// Compares the two display modes in a strict - weak - order(SWO) fashion.
bool CompareSWO(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs);