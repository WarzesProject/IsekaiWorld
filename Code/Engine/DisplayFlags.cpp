#include "stdafx.h"
#include "DisplayFlags.h"
//-----------------------------------------------------------------------------
bool operator==(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs)
{
	return (
		lhs.resolution == rhs.resolution &&
		lhs.refreshRate == rhs.refreshRate
	);
}
//-----------------------------------------------------------------------------
bool operator!=(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs)
{
	return !(lhs == rhs);
}
//-----------------------------------------------------------------------------
bool CompareSWO(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs)
{
	const auto lhsNumPixels = lhs.resolution.width * lhs.resolution.height;
	const auto rhsNumPixels = rhs.resolution.width * rhs.resolution.height;

	if (lhsNumPixels < rhsNumPixels)
		return true;
	if (lhsNumPixels > rhsNumPixels)
		return false;

	return (lhs.refreshRate < rhs.refreshRate);
}
//-----------------------------------------------------------------------------
// Computes the greatest common divisor (GCD) for the two parameters
// see https://stackoverflow.com/questions/10956543/gcd-function-in-c-sans-cmath-library
static uint32_t ComputeGCD(uint32_t a, uint32_t b)
{
	while (b != 0)
	{
		auto r = a % b;
		a = b;
		b = r;
	}
	return a;
}
//-----------------------------------------------------------------------------
Extent2D GetExtentRatio(const Extent2D &extent)
{
	auto gcd = ComputeGCD(extent.width, extent.height);
	return { extent.width / gcd, extent.height / gcd };
}
//-----------------------------------------------------------------------------