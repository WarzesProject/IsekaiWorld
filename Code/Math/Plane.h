#pragma once

#include "Vector.h"

template <typename T>
class Plane final
{
public:
	constexpr Plane() noexcept {}

	constexpr Plane(const T a, const T b, const T c, const T d) noexcept : v{ a, b, c, d } {}

	inline T& operator[](size_t index) noexcept { return v[index]; }
	constexpr T operator[](size_t index) const noexcept { return v[index]; }

	constexpr bool operator==(const Plane &plane) const noexcept
	{
		return v[0] == plane.v[0] && v[1] == plane.v[1] && v[2] == plane.v[2] && v[3] == plane.v[3];
	}

	constexpr bool operator!=(const Plane &plane) const noexcept
	{
		return v[0] != plane.v[0] || v[1] != plane.v[1] || v[2] != plane.v[2] || v[3] != plane.v[3];
	}
	
	inline T& a() noexcept { return v[0]; }
	constexpr T a() const noexcept { return v[0]; }

	inline T& b() noexcept { return v[1]; }
	constexpr T b() const noexcept { return v[1]; }

	inline T& c() noexcept { return v[2]; }
	constexpr T c() const noexcept { return v[2]; }

	inline T& d() noexcept { return v[3]; }
	constexpr T d() const noexcept { return v[3]; }

	inline void Flip() noexcept
	{
		v[0] = -v[0];
		v[1] = -v[1];
		v[2] = -v[2];
		v[3] = -v[3];
	}

	template<size_t N, typename std::enable_if<N >= 3>::type * = nullptr>
	constexpr T Dot(const Vector<N, T> &vec) const noexcept
	{
		return v[0] * vec.v[0] + v[1] * vec.v[1] + v[2] * vec.v[2] + v[3];
	}

	void Normalize() noexcept
	{
		T n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		if( n == T(1) ) // already normalized
			return;

		n = std::sqrt(n);
		if( n <= std::numeric_limits<T>::min() ) // too close to zero
			return;

		n = T(1) / n;
		v[0] *= n;
		v[1] *= n;
		v[2] *= n;
		v[3] *= n;
	}

	Plane Normalized() const noexcept
	{
		T n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		if( n == T(1) ) // already normalized
			return *this;

		n = std::sqrt(n);
		if( n <= std::numeric_limits<T>::min() ) // too close to zero
			return *this;

		n = T(1) / n;
		return Plane(v[0] * n, v[1] * n, v[2] * n, v[3] * n);
	}
	
	static inline Plane MakeFrustumPlane(const T a, const T b, const T c, const T d) noexcept
	{
		T n = std::sqrt(a * a + b * b + c * c);
		if( n <= std::numeric_limits<T>::min() ) // too close to zero
			return Plane();

		n = T(1) / n;
		return Plane(a * n, b * n, c * n, d * n);
	}

	T v[4]{ 0 };
};

using PlaneF = Plane<float>;