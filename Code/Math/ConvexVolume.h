#pragma once

#include "Plane.h"
#include "Box.h"

template <typename T> 
class ConvexVolume final
{
public:
	ConvexVolume() noexcept {}

	explicit ConvexVolume(const std::vector<Plane<T>> &initPlanes) noexcept : planes(initPlanes) {}

	template<size_t N, typename std::enable_if<N >= 3>::type * = nullptr>
	inline auto IsPointInside(const Vector<N, T> &position) const noexcept
	{
		for( const Plane<T> &plane : planes )
			if( plane.Dot(position) < T(0) )
				return false;

		return true;
	}

	template<size_t N, typename std::enable_if<N >= 3>::type * = nullptr>
	inline auto IsSphereInside(const Vector<N, T> &position, const T radius) const noexcept
	{
		for( const Plane<T> &plane : planes )
			if( plane.Dot(position) < -radius )
				return false;

		return true;
	}

	auto IsBoxInside(const Box<3, T> &box) const noexcept
	{
		const Vector<4, T> leftBottomBack(box.min.v[0], box.min.v[1], box.min.v[2], 1);
		const Vector<4, T> leftBottomFront(box.min.v[0], box.min.v[1], box.max.v[2], 1);
		const Vector<4, T> leftTopBack(box.min.v[0], box.max.v[1], box.min.v[2], 1);
		const Vector<4, T> leftTopFront(box.min.v[0], box.max.v[1], box.max.v[2], 1);
		const Vector<4, T> rightBottomBack(box.max.v[0], box.min.v[1], box.min.v[2], 1);
		const Vector<4, T> rightBottomFront(box.max.v[0], box.min.v[1], box.max.v[2], 1);
		const Vector<4, T> rightTopBack(box.max.v[0], box.max.v[1], box.min.v[2], 1);
		const Vector<4, T> rightTopFront(box.max.v[0], box.max.v[1], box.max.v[2], 1);

		for( const Plane<T> &plane : planes )
			if( plane.Dot(leftBottomBack) < T(0) &&
				plane.Dot(leftBottomFront) < T(0) &&
				plane.Dot(leftTopBack) < T(0) &&
				plane.Dot(leftTopFront) < T(0) &&
				plane.Dot(rightBottomBack) < T(0) &&
				plane.Dot(rightBottomFront) < T(0) &&
				plane.Dot(rightTopBack) < T(0) &&
				plane.Dot(rightTopFront) < T(0) )
				return false;

		return true;
	}

	std::vector<Plane<T>> planes;
};

using ConvexVolumeF = ConvexVolume<float>;