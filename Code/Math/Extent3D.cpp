#include "stdafx.h"
#include "Extent3D.h"
#include "CoreMath.h"
//-----------------------------------------------------------------------------
Extent3D operator+(const Extent3D& lhs, const Extent3D& rhs)
{
	return Extent3D
	{
		AddUInt32Clamped(lhs.width , rhs.width),
		AddUInt32Clamped(lhs.height, rhs.height),
		AddUInt32Clamped(lhs.depth , rhs.depth)
	};
}
//-----------------------------------------------------------------------------
Extent3D operator-(const Extent3D& lhs, const Extent3D& rhs)
{
	return Extent3D
	{
		SubUInt32Clamped(lhs.width , rhs.width),
		SubUInt32Clamped(lhs.height, rhs.height),
		SubUInt32Clamped(lhs.depth , rhs.depth)
	};
}
//-----------------------------------------------------------------------------