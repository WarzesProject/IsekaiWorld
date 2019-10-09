#pragma once

#include "Extent2D.h"

struct DisplayModeDescriptor
{
	// Display resolution (in pixels).
	Extent2D resolution;
	// Display refresh rate (in Hz).
	uint32_t refreshRate = 0;
};

bool operator==(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs);
bool operator!=(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs);

// Compares the two display modes in a strict - weak - order(SWO) fashion.
bool CompareSWO(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs);

// Returns the ratio of the specified extent as another extent, i.e. all attributes are divided by their greatest common divisor.
// remarks This can be used to print out a display mode resolution in a better format (e.g. "16:9" rather than "1920:1080").
Extent2D GetExtentRatio(const Extent2D &extent);