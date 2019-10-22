#include "stdafx.h"
#include "Offset3D.h"
#include "CoreMath.h"
//-----------------------------------------------------------------------------
Offset3D operator+(const Offset3D &lhs, const Offset3D &rhs)
{
	return Offset3D
	{
		AddInt32Clamped(lhs.x, rhs.x),
		AddInt32Clamped(lhs.y, rhs.y),
		AddInt32Clamped(lhs.z, rhs.z)
	};
}
//-----------------------------------------------------------------------------
Offset3D operator-(const Offset3D &lhs, const Offset3D &rhs)
{
	return Offset3D
	{
		SubInt32Clamped(lhs.x, rhs.x),
		SubInt32Clamped(lhs.y, rhs.y),
		SubInt32Clamped(lhs.z, rhs.z)
	};
}
//-----------------------------------------------------------------------------