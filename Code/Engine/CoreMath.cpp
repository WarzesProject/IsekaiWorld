#include "stdafx.h"
#include "CoreMath.h"
//-----------------------------------------------------------------------------
uint32_t AddUInt32Clamped(uint32_t lhs, uint32_t rhs)
{
	constexpr auto max = std::numeric_limits<uint32_t>::max();

	uint64_t x = lhs;
	x += rhs;

	if (x > static_cast<uint64_t>(max))
		return max;
	else
		return lhs + rhs;
}
//-----------------------------------------------------------------------------
uint32_t SubUInt32Clamped(uint32_t lhs, uint32_t rhs)
{
	if (rhs < lhs)
		return lhs - rhs;
	else
		return 0;
}
//-----------------------------------------------------------------------------