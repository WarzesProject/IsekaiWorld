#pragma once

#include "Size.h"

template <size_t N, typename T> 
class Box final
{
public:
	Box() noexcept
	{
		for( T& v : min.v )
			v = std::numeric_limits<T>::max();
		for( T& v : max.v )
			v = std::numeric_limits<T>::lowest();
	}

	constexpr Box(const Vector<N, T> &initMin, const Vector<N, T> &initMax) noexcept : min(initMin), max(initMax) {}

	template <size_t N2>
	explicit Box(const Box<N2, T> &box) noexcept : min(Vector<N, T>(box.min)), max(Vector<N, T>(box.max)) {}

	constexpr const Box operator+(const Vector<N, T> &v) const noexcept
	{
		return Box(min + v, max + v);
	}

	inline Box& operator+=(const Vector<N, T> &v) noexcept
	{
		min += v;
		max += v;
		return *this;
	}

	constexpr const Box operator-(const Vector<N, T> &v) const noexcept
	{
		return Box(min - v, max - v);
	}

	inline Box& operator-=(const Vector<N, T> &v) noexcept
	{
		min -= v;
		max -= v;
		return *this;
	}

	inline Vector<N, T> GetCenter() const noexcept
	{
		return (min + max) / T(2);
	}

	bool Intersects(const Box &aabb) const noexcept
	{
		for( size_t i = 0; i < N; ++i )
			if( aabb.min.v[0] > max.v[0] ) return false;
		for( size_t i = 0; i < N; ++i )
			if( aabb.max.v[0] < min.v[0] ) return false;
		return true;
	}

	bool ContainsPoint(const Vector<N, T> &point) const noexcept
	{
		for( size_t i = 0; i < N; ++i )
			if( point.v[i] < min.v[i] ) return false;
		for( size_t i = 0; i < N; ++i )
			if( point.v[i] > max.v[i] ) return false;
		return true;
	}

	void Merge(const Box &box) noexcept
	{
		for( size_t i = 0; i < N; ++i )
			min.v[i] = std::min(min.v[i], box.min.v[i]);
		for( size_t i = 0; i < N; ++i )
			max.v[i] = std::max(max.v[i], box.max.v[i]);
	}

	inline void Reset() noexcept
	{
		for( T& v : min.v )
			v = std::numeric_limits<T>::max();
		for( T& v : max.v )
			v = std::numeric_limits<T>::lowest();
	}

	inline auto IsEmpty() const noexcept
	{
		for( size_t i = 0; i < N; ++i )
			if( min.v[i] > max.v[i] )
				return true;
		return false;
	}

	inline void InsertPoint(const Vector<N, T> &point) noexcept
	{
		for( size_t i = 0; i < N; ++i )
			if( point.v[i] < min.v[i] ) min.v[i] = point.v[i];
		for( size_t i = 0; i < N; ++i )
			if( point.v[i] > max.v[i] ) max.v[i] = point.v[i];
	}
	
	inline Size<N, T> GetSize() const noexcept
	{
		return Size<N, T>(max - min);
	}

	Vector<N, T> min;
	Vector<N, T> max;
};

using Box2F = Box<2, float>;
using Box3F = Box<3, float>;