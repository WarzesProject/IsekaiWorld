#pragma once

struct Offset2D
{
	Offset2D() = default;
	Offset2D(const Offset2D&) = default;
	Offset2D(int32_t X, int32_t Y) : x{ X }, y{ Y } {}

	int32_t x = 0;
	int32_t y = 0;
};

Offset2D operator+(const Offset2D &lhs, const Offset2D &rhs);
Offset2D operator-(const Offset2D &lhs, const Offset2D &rhs);

inline bool operator==(const Offset2D &lhs, const Offset2D &rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

inline bool operator!=(const Offset2D &lhs, const Offset2D &rhs)
{
	return !(lhs == rhs);
}