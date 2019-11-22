#pragma once

#include "Size.h"

template <typename T>
class Rect final
{
public:
	constexpr Rect() noexcept {}
	constexpr Rect(const T width, const T height) noexcept : size(width, height) {}
	constexpr Rect(const T x, const T y, const T width, const T height) noexcept : position(x, y), size(width, height) {}
	Rect(const Vector<2, T> &initPosition, const T width, const T height) noexcept : position(initPosition), size(width, height) {}
	Rect(const Vector<2, T> &initPosition, const Size<2, T> &initSize) noexcept : position(initPosition), size(initSize) {}

	constexpr bool operator==(const Rect &other) const noexcept
	{
		return position.v[0] == other.position.v[0] && size.v[0] == other.size.v[0] &&
			position.v[1] == other.position.v[1] && size.v[1] == other.size.v[1];
	}

	constexpr bool operator!=(const Rect &other) const noexcept
	{
		return position.v[0] != other.position.v[0] || size.v[0] != other.size.v[0] ||
			position.v[1] != other.position.v[1] || size.v[1] != other.size.v[1];
	}

	inline const Rect operator*(const T scalar) const noexcept
	{
		return Rect(position.v[0] * scalar, position.v[1] * scalar, size.v[0] * scalar, size.v[1] * scalar);
	}

	inline Rect& operator*=(const T scalar) noexcept
	{
		position.v[0] *= scalar;
		position.v[1] *= scalar;
		size.v[0] *= scalar;
		size.v[1] *= scalar;
		return *this;
	}

	inline const Rect operator/(const T scalar) const noexcept
	{
		return Rect(position.v[0] / scalar, position.v[1] / scalar, size.v[0] / scalar, size.v[1] / scalar);
	}

	inline Rect& operator/=(const T scalar) noexcept
	{
		position.v[0] /= scalar;
		position.v[1] /= scalar;
		size.v[0] /= scalar;
		size.v[1] /= scalar;
		return *this;
	}

	inline auto IsEmpty() const noexcept
	{
		return size.IsZero();
	}

	inline void SetPosition(const T x, const T y) noexcept
	{
		position.v[0] = x;
		position.v[1] = y;
	}

	inline void SetPosition(const Vector<2, T> &newPosition) noexcept
	{
		position = newPosition;
	}

	constexpr T Left() const noexcept
	{
		return position.v[0];
	}

	constexpr T Bottom() const noexcept
	{
		return position.v[1];
	}

	constexpr T Right() const noexcept
	{
		return position.v[0] + size.v[0];
	}

	constexpr T Top() const noexcept
	{
		return position.v[1] + size.v[1];
	}

	constexpr Vector<2, T> BottomLeft() const noexcept
	{
		return position;
	}

	constexpr Vector<2, T> TopRight() const noexcept
	{
		return Vector<2, T>(position.v[0] + size.v[0], position.v[1] + size.v[1]);
	}

	constexpr bool ContainsPoint(const T x, const T y) const noexcept
	{
		return x >= position.v[0] && x <= (position.v[0] + size.v[0]) &&
			y >= position.v[1] && y <= (position.v[1] + size.v[1]);
	}

	constexpr bool ContainsPoint(const Vector<2, T> &point) const noexcept
	{
		return point.v[0] >= position.v[0] && point.v[0] <= (position.v[0] + size.v[0]) &&
			point.v[1] >= position.v[1] && point.v[1] <= (position.v[1] + size.v[1]);
	}

	constexpr bool Contains(const T x, const T y, const T width, const T height) const noexcept
	{
		return ContainsPoint(x, y) && ContainsPoint(x + width, y + height);
	}

	constexpr bool Contains(const Rect &r) const noexcept
	{
		return Contains(r.position.v[0], r.position.v[1], r.size.v[0], r.size.v[1]);
	}

	constexpr bool Intersects(const T x, const T y, const T width, const T height) const noexcept
	{
		T t;
		if( (t = x - position.v[0]) > size.v[0] || -t > width )
			return false;
		if( (t = y - position.v[1]) > size.v[1] || -t > height )
			return false;
		return true;
	}

	constexpr bool Intersects(const Rect &r) const noexcept
	{
		return Intersects(r.position.v[0], r.position.v[1], r.size.v[0], r.size.v[1]);
	}

	static bool Intersect(const Rect &r1, const Rect &r2, Rect &dst) noexcept
	{
		const T xmin = std::max(r1.position.v[0], r2.position.v[0]);
		const T xmax = std::min(r1.right(), r2.right());
		if( xmax > xmin )
		{
			const T ymin = std::max(r1.position.v[1], r2.position.v[1]);
			const T ymax = std::min(r1.bottom(), r2.bottom());
			if( ymax > ymin )
			{
				dst.position.v[0] = xmin;
				dst.position.v[1] = ymin;
				dst.size.v[0] = xmax - xmin;
				dst.size.v[1] = ymax - ymin;
				return true;
			}
		}

		dst.position.v[0] = dst.position.v[1] = dst.size.v[0] = dst.size.v[1] = 0;
		return false;
	}

	static void Combine(const Rect &r1, const Rect &r2, Rect &dst) noexcept
	{
		dst.position.v[0] = std::min(r1.position.v[0], r2.position.v[0]);
		dst.position.v[1] = std::min(r1.position.v[1], r2.position.v[1]);
		dst.size.v[0] = std::max(r1.position.v[0] + r1.size.v[0], r2.position.v[0] + r2.size.v[0]) - dst.position.v[0];
		dst.size.v[1] = std::max(r1.position.v[1] + r1.size.v[1], r2.position.v[1] + r2.size.v[1]) - dst.position.v[1];
	}

	inline void Inflate(const T horizontalAmount, const T verticalAmount) noexcept
	{
		position.v[0] -= horizontalAmount;
		position.v[1] -= verticalAmount;
		size.v[0] += horizontalAmount * T(2);
		size.v[1] += verticalAmount * T(2);
	}	

	Vector<2, T> position;
	Size<2, T> size;
};

using RectF = Rect<float>;