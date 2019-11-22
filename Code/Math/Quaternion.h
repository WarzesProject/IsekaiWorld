#pragma once

#include "Vector.h"

template <typename T> 
class Quaternion final
{
public:
	constexpr Quaternion() noexcept {}

	constexpr Quaternion(const T x, const T y, const T z, const T w) noexcept : v{ x, y, z, w } {}

	inline T& operator[](size_t index) noexcept { return v[index]; }
	constexpr T operator[](size_t index) const noexcept { return v[index]; }

	constexpr const Quaternion operator*(const Quaternion &q) const noexcept
	{
		return Quaternion(
			v[0] * q.v[3] + v[1] * q.v[2] - v[2] * q.v[1] + v[3] * q.v[0],
			-v[0] * q.v[2] + v[1] * q.v[3] + v[2] * q.v[0] + v[3] * q.v[1],
			v[0] * q.v[1] - v[1] * q.v[0] + v[2] * q.v[3] + v[3] * q.v[2],
			-v[0] * q.v[0] - v[1] * q.v[1] - v[2] * q.v[2] + v[3] * q.v[3]);
	}

	inline Quaternion& operator*=(const Quaternion &q) noexcept
	{
		const T tempX = v[0] * q.v[3] + v[1] * q.v[2] - v[2] * q.v[1] + v[3] * q.v[0];
		const T tempY = -v[0] * q.v[2] + v[1] * q.v[3] + v[2] * q.v[0] + v[3] * q.v[1];
		const T tempZ = v[0] * q.v[1] - v[1] * q.v[0] + v[2] * q.v[3] + v[3] * q.v[2];
		const T tempW = -v[0] * q.v[0] - v[1] * q.v[1] - v[2] * q.v[2] + v[3] * q.v[3];

		v[0] = tempX;
		v[1] = tempY;
		v[2] = tempZ;
		v[3] = tempW;

		return *this;
	}

	constexpr const Quaternion operator*(const T scalar) const noexcept
	{
		return Quaternion(
			v[0] * scalar,
			v[1] * scalar,
			v[2] * scalar,
			v[3] * scalar);
	}

	inline Quaternion& operator*=(const T scalar) noexcept
	{
		v[0] *= scalar;
		v[1] *= scalar;
		v[2] *= scalar;
		v[3] *= scalar;

		return *this;
	}

	constexpr const Quaternion operator/(const T scalar) const noexcept
	{
		return Quaternion(
			v[0] / scalar,
			v[1] / scalar,
			v[2] / scalar,
			v[3] / scalar);
	}

	inline Quaternion& operator/=(const T scalar) noexcept
	{
		v[0] /= scalar;
		v[1] /= scalar;
		v[2] /= scalar;
		v[3] /= scalar;

		return *this;
	}

	constexpr const Quaternion operator-() const noexcept
	{
		return Quaternion(-v[0], -v[1], -v[2], -v[3]);
	}

	constexpr const Quaternion operator+(const Quaternion &q) const noexcept
	{
		return Quaternion(
			v[0] + q.v[0],
			v[1] + q.v[1],
			v[2] + q.v[2],
			v[3] + q.v[3]);
	}

	inline Quaternion& operator+=(const Quaternion &q) noexcept
	{
		v[0] += q.v[0];
		v[1] += q.v[1];
		v[2] += q.v[2];
		v[3] += q.v[3];

		return *this;
	}

	constexpr const Quaternion operator-(const Quaternion &q) const noexcept
	{
		return Quaternion(
			v[0] - q.v[0],
			v[1] - q.v[1],
			v[2] - q.v[2],
			v[3] - q.v[3]);
	}

	inline Quaternion& operator-=(const Quaternion &q) noexcept
	{
		v[0] -= q.v[0];
		v[1] -= q.v[1];
		v[2] -= q.v[2];
		v[3] -= q.v[3];

		return *this;
	}

	constexpr bool operator==(const Quaternion &q) const noexcept
	{
		return v[0] == q.v[0] && v[1] == q.v[1] && v[2] == q.v[2] && v[3] == q.v[3];
	}

	constexpr bool operator!=(const Quaternion &q) const noexcept
	{
		return v[0] != q.v[0] || v[1] != q.v[1] || v[2] != q.v[2] || v[3] != q.v[3];
	}

	inline const Vector<3, T> operator*(const Vector<3, T> &vector) const noexcept
	{
		return RotateVector(vector);
	}

	inline T& x() noexcept { return v[0]; }
	constexpr T x() const noexcept { return v[0]; }

	inline T& y() noexcept { return v[1]; }
	constexpr T y() const noexcept { return v[1]; }

	inline T& z() noexcept { return v[2]; }
	constexpr T z() const noexcept { return v[2]; }

	inline T& w() noexcept { return v[3]; }
	constexpr T w() const noexcept { return v[3]; }

	static constexpr Quaternion Identity() noexcept
	{
		return Quaternion(0, 0, 0, 1);
	}	

	inline void Negate() noexcept
	{
		v[0] = -v[0];
		v[1] = -v[1];
		v[2] = -v[2];
		v[3] = -v[3];
	}

	inline void Conjugate() noexcept
	{
		v[0] = -v[0];
		v[1] = -v[1];
		v[2] = -v[2];
	}

	inline void Invert() noexcept
	{
		const T n2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]; // norm squared

		if( n2 <= std::numeric_limits<T>::min() )
			return;

		// conjugate divided by norm squared
		v[0] = -v[0] / n2;
		v[1] = -v[1] / n2;
		v[2] = -v[2] / n2;
		v[3] = v[3] / n2;
	}

	inline auto GetNorm() const noexcept
	{
		const T n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		if( n == T(1) ) // already normalized
			return 1;

		return std::sqrt(n);
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

	Quaternion Normalized() const noexcept
	{
		T n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		if( n == T(1) ) // already normalized
			return *this;

		n = std::sqrt(n);
		if( n <= std::numeric_limits<T>::min() ) // too close to zero
			return *this;

		n = T(1) / n;
		return *this * n;
	}

	void Rotate(const T angle, const Vector<3, T> &axis) noexcept
	{
		const auto normalizedAxis = axis.Normalized();

		const T cosAngle = std::cos(angle / T(2));
		const T sinAngle = std::sin(angle / T(2));

		v[0] = normalizedAxis.v[0] * sinAngle;
		v[1] = normalizedAxis.v[1] * sinAngle;
		v[2] = normalizedAxis.v[2] * sinAngle;
		v[3] = cosAngle;
	}

	void GetRotation(T &angle, Vector<3, T> &axis) const noexcept
	{
		angle = T(2) * std::acos(v[3]);
		const T s = std::sqrt(T(1) - v[3] * v[3]);
		if( s <= std::numeric_limits<T>::min() ) // too close to zero
		{
			axis.v[0] = v[0];
			axis.v[1] = v[1];
			axis.v[2] = v[2];
		}
		else
		{
			axis.v[0] = v[0] / s;
			axis.v[1] = v[1] / s;
			axis.v[2] = v[2] / s;
		}
	}

	Vector<3, T> GetEulerAngles() const noexcept
	{
		Vector<3, T> result;
		result.v[0] = std::atan2(2 * (v[1] * v[2] + v[3] * v[0]), v[3] * v[3] - v[0] * v[0] - v[1] * v[1] + v[2] * v[2]);
		result.v[1] = std::asin(-2 * (v[0] * v[2] - v[3] * v[1]));
		result.v[2] = std::atan2(2 * (v[0] * v[1] + v[3] * v[2]), v[3] * v[3] + v[0] * v[0] - v[1] * v[1] - v[2] * v[2]);
		return result;
	}

	inline auto GetEulerAngleX() const noexcept
	{
		return std::atan2(T(2) * (v[1] * v[2] + v[3] * v[0]), v[3] * v[3] - v[0] * v[0] - v[1] * v[1] + v[2] * v[2]);
	}

	inline auto GetEulerAngleY() const noexcept
	{
		return std::asin(T(-2) * (v[0] * v[2] - v[3] * v[1]));
	}

	inline auto GetEulerAngleZ() const noexcept
	{
		return std::atan2(T(2) * (v[0] * v[1] + v[3] * v[2]), v[3] * v[3] + v[0] * v[0] - v[1] * v[1] - v[2] * v[2]);
	}

	void SetEulerAngles(const Vector<3, T> &angles) noexcept
	{
		T angle = angles.v[0] / T(2);
		const T sr = std::sin(angle);
		const T cr = std::cos(angle);

		angle = angles.v[1] / T(2);
		const T sp = std::sin(angle);
		const T cp = std::cos(angle);

		angle = angles.v[2] / T(2);
		const T sy = std::sin(angle);
		const T cy = std::cos(angle);

		const T cpcy = cp * cy;
		const T spcy = sp * cy;
		const T cpsy = cp * sy;
		const T spsy = sp * sy;

		v[0] = sr * cpcy - cr * spsy;
		v[1] = cr * spcy + sr * cpsy;
		v[2] = cr * cpsy - sr * spcy;
		v[3] = cr * cpcy + sr * spsy;
	}

	inline Vector<3, T> RotateVector(const Vector<3, T> &vector) const noexcept
	{
		const Vector<3, T> q(v[0], v[1], v[2]);
		const Vector<3, T> t = T(2) * q.Cross(vector);
		return vector + (v[3] * t) + q.Cross(t);
	}

	inline Vector<3, T> GetRightVector() const noexcept
	{
		return RotateVector(Vector<3, T>(1, 0, 0));
	}

	inline Vector<3, T> GetUpVector() const noexcept
	{
		return RotateVector(Vector<3, T>(0, 1, 0));
	}

	inline Vector<3, T> GetForwardVector() const noexcept
	{
		return RotateVector(Vector<3, T>(0, 0, 1));
	}

	inline Quaternion& Lerp(const Quaternion &q1, const Quaternion &q2, T t) noexcept
	{
		*this = (q1 * (T(1) - t)) + (q2 * t);
		return *this;
	}

	T v[4]{ 0 };
};

using QuaternionF = Quaternion<float>;