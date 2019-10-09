#include "stdafx.h"
#include "DisplayFlags.h"
//-----------------------------------------------------------------------------
bool operator==(const DisplayModeDescriptor &lhs, const DisplayModeDescriptor &rhs)
{
	return (
		lhs.width == rhs.width &&
		lhs.height == rhs.height &&
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
	const auto lhsNumPixels = lhs.width * lhs.height;
	const auto rhsNumPixels = rhs.width * rhs.height;

	if (lhsNumPixels < rhsNumPixels)
		return true;
	if (lhsNumPixels > rhsNumPixels)
		return false;

	return (lhs.refreshRate < rhs.refreshRate);
}
//-----------------------------------------------------------------------------