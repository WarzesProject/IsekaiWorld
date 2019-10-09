#pragma once

struct Offset3D
{
	Offset3D() = default;
	Offset3D(const Offset3D&) = default;
	Offset3D(int32_t X, int32_t Y, int32_t Z) : x{ X }, y{ Y }, z{ Z } {}

	int32_t x = 0;
	int32_t y = 0;
	int32_t z = 0;
};

Offset3D operator+(const Offset3D &lhs, const Offset3D &rhs);
Offset3D operator-(const Offset3D &lhs, const Offset3D &rhs);

...llgl..