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
int32_t ClampToInt32(int64_t x)
{
	constexpr auto xMin = static_cast<int64_t>(std::numeric_limits<int32_t>::min());
	constexpr auto xMax = static_cast<int64_t>(std::numeric_limits<int32_t>::max());

	x = std::max(xMin, std::min(x, xMax));

	return static_cast<int32_t>(x);
}
//-----------------------------------------------------------------------------
int32_t AddInt32Clamped(int32_t lhs, int32_t rhs)
{
	int64_t x = lhs;
	x += rhs;
	return ClampToInt32(x);
}
//-----------------------------------------------------------------------------
int32_t SubInt32Clamped(int32_t lhs, int32_t rhs)
{
	int64_t x = lhs;
	x -= rhs;
	return ClampToInt32(x);
}
//-----------------------------------------------------------------------------