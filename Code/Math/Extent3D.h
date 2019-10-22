#pragma once

struct Extent3D
{
	Extent3D() = default;
	Extent3D(const Extent3D&) = default;
	Extent3D(uint32_t Width, uint32_t Height, uint32_t Depth) : width{ Width }, height{ Height }, depth{ Depth } {}

	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t depth = 0;
};

Extent3D operator+(const Extent3D &lhs, const Extent3D &rhs);
Extent3D operator-(const Extent3D &lhs, const Extent3D &rhs);

inline bool operator==(const Extent3D &lhs, const Extent3D &rhs)
{
	return (lhs.width == rhs.width && lhs.height == rhs.height && lhs.depth == rhs.depth);
}

inline bool operator!=(const Extent3D &lhs, const Extent3D &rhs)
{
	return !(lhs == rhs);
}