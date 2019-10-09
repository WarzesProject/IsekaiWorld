#include "stdafx.h"
#include "Offset2D.h"
#include "CoreMath.h"
//-----------------------------------------------------------------------------
Offset2D operator+(const Offset2D &lhs, const Offset2D &rhs)
{
	return Offset2D
	{
		AddInt32Clamped(lhs.x, rhs.x),
		AddInt32Clamped(lhs.y, rhs.y)
	};
}
//-----------------------------------------------------------------------------
Offset2D operator-(const Offset2D &lhs, const Offset2D &rhs)
{
	return Offset2D
	{
		SubInt32Clamped(lhs.x, rhs.x),
		SubInt32Clamped(lhs.y, rhs.y)
	};
}
//-----------------------------------------------------------------------------