#pragma once

struct Extent2D
{
	Extent2D() = default;
	Extent2D(const Extent2D&) = default;
	Extent2D(uint32_t Width, uint32_t Height) :	width{ Width },	height{ Height } {}

	uint32_t width = 0; 
	uint32_t height = 0;
};

Extent2D operator+(const Extent2D &lhs, const Extent2D &rhs);
Extent2D operator-(const Extent2D &lhs, const Extent2D &rhs);

inline bool operator==(const Extent2D &lhs, const Extent2D &rhs)
{
	return (lhs.width == rhs.width && lhs.height == rhs.height);
}

inline bool operator!=(const Extent2D &lhs, const Extent2D &rhs)
{
	return !(lhs == rhs);
}