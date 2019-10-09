#include "stdafx.h"
#include "Extent2D.h"
#include "CoreMath.h"
//-----------------------------------------------------------------------------
Extent2D operator+(const Extent2D &lhs, const Extent2D &rhs)
{
	return Extent2D
	{
		AddUInt32Clamped(lhs.width , rhs.width),
		AddUInt32Clamped(lhs.height, rhs.height)
	};
}
//-----------------------------------------------------------------------------
Extent2D operator-(const Extent2D &lhs, const Extent2D &rhs)
{
	return Extent2D
	{
		SubUInt32Clamped(lhs.width , rhs.width),
		SubUInt32Clamped(lhs.height, rhs.height)
	};
}
//-----------------------------------------------------------------------------