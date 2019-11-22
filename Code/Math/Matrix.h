#pragma once

#include "ConvexVolume.h"
#include "Quaternion.h"

template <size_t C, size_t R, typename T> 
class Matrix final
{
public:
	constexpr Matrix() noexcept {}

	template <typename ...A>
	constexpr Matrix(A... args) noexcept : m{ static_cast<T>(args)... } {}

	T& operator[](size_t index) noexcept { return m[index]; }
	constexpr T operator[](size_t index) const noexcept { return m[index]; }

	inline const Matrix operator+(const Matrix &matrix) const noexcept
	{
		Matrix result(*this);
		result.Add(matrix, result);
		return result;
	}

	inline Matrix& operator+=(const Matrix &matrix) noexcept
	{
		Add(matrix);
		return *this;
	}

	inline const Matrix operator-(const Matrix &matrix) const noexcept
	{
		Matrix result(*this);
		result.Subtract(matrix, result);
		return result;
	}

	inline Matrix& operator-=(const Matrix &matrix) noexcept
	{
		Subtract(matrix);
		return *this;
	}

	inline const Matrix operator-() const noexcept
	{
		Matrix result(*this);
		Negate(result);
		return result;
	}

	inline const Matrix operator*(const Matrix &matrix) const noexcept
	{
		Matrix result(*this);
		result.Multiply(matrix, result);
		return result;
	}

	inline Matrix& operator*=(const Matrix &matrix) noexcept
	{
		Multiply(matrix);
		return *this;
	}

	inline bool operator==(const Matrix &matrix) noexcept
	{
		for( size_t i = 0; i < C * R; ++i )
			if( m[i] != matrix.m[i] )
				return false;

		return true;
	}

	inline bool operator!=(const Matrix &matrix) noexcept
	{
		for( size_t i = 0; i < C * R; ++i )
			if( m[i] != matrix.m[i] )
				return true;

		return false;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline const Vector<3, T> operator*(const Vector<3, T> & v) noexcept
	{
		Vector<3, T> x;
		transformVector(v, x);
		return x;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline const Vector<4, T> operator*(const Vector<4, T> & v) noexcept
	{
		Vector<4, T> x;
		transformVector(v, x);
		return x;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	static constexpr Matrix Identity() noexcept
	{
		return Matrix(
			T(1), T(0), T(0), T(0),
			T(0), T(1), T(0), T(0),
			T(0), T(0), T(1), T(0),
			T(0), T(0), T(0), T(1));
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetLookAt(const Vector<3, T> &eyePosition,
		const Vector<3, T> &targetPosition,
		const Vector<3, T> &up) noexcept
	{
		setLookAt(eyePosition.v[0], eyePosition.v[1], eyePosition.v[2],
			targetPosition.v[0], targetPosition.v[1], targetPosition.v[2],
			up.v[0], up.v[1], up.v[2]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetLookAt(const T eyePositionX, const T eyePositionY, const T eyePositionZ,
		const T targetPositionX, const T targetPositionY, const T targetPositionZ,
		const T upX, const T upY, const T upZ) noexcept
	{
		const Vector<3, T> eye(eyePositionX, eyePositionY, eyePositionZ);
		const Vector<3, T> target(targetPositionX, targetPositionY, targetPositionZ);
		Vector<3, T> up(upX, upY, upZ);
		up.Normalize();

		Vector<3, T> zaxis = target - eye;
		zaxis.Normalize();

		Vector<3, T> xaxis = up.Cross(zaxis);
		xaxis.Normalize();

		Vector<3, T> yaxis = zaxis.Cross(xaxis);
		yaxis.Normalize();

		m[0] = xaxis.v[0];
		m[1] = yaxis.v[0];
		m[2] = zaxis.v[0];
		m[3] = T(0);

		m[4] = xaxis.v[1];
		m[5] = yaxis.v[1];
		m[6] = zaxis.v[1];
		m[7] = T(0);

		m[8] = xaxis.v[2];
		m[9] = yaxis.v[2];
		m[10] = zaxis.v[2];
		m[11] = T(0);

		m[12] = xaxis.Dot(-eye);
		m[13] = yaxis.Dot(-eye);
		m[14] = zaxis.Dot(-eye);
		m[15] = T(1);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetPerspective(const T fieldOfView, const T aspectRatio, const T zNearPlane, const T zFarPlane) noexcept
	{
		assert(zFarPlane != zNearPlane);

		const T theta = fieldOfView / T(2);
		if( std::fabs(std::fmod(theta, pi<T> / T(2))) <= std::numeric_limits<T>::min() )
			return;

		const T divisor = tan(theta);
		assert(divisor);
		const T factor = T(1) / divisor;

		SetZero();

		assert(aspectRatio);
		m[0] = (T(1) / aspectRatio) * factor;
		m[5] = factor;
		m[10] = zFarPlane / (zFarPlane - zNearPlane);
		m[11] = T(1);
		m[14] = -zNearPlane * zFarPlane / (zFarPlane - zNearPlane);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetOrthographicFromSize(const T width, const T height, const T zNearPlane, const T zFarPlane) noexcept
	{
		const T halfWidth = width / T(2);
		const T halfHeight = height / T(2);
		SetOrthographicOffCenter(-halfWidth, halfWidth,
			-halfHeight, halfHeight,
			zNearPlane, zFarPlane);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetOrthographicOffCenter(const T left, const T right,
		const T bottom, const T top,
		const T zNearPlane, const T zFarPlane) noexcept
	{
		assert(right != left);
		assert(top != bottom);
		assert(zFarPlane != zNearPlane);

		SetZero();

		m[0] = T(2) / (right - left);
		m[5] = T(2) / (top - bottom);
		m[10] = T(1) / (zFarPlane - zNearPlane);
		m[12] = (left + right) / (left - right);
		m[13] = (bottom + top) / (bottom - top);
		m[14] = zNearPlane / (zNearPlane - zFarPlane);
		m[15] = T(1);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	void SetScale(const Vector<2, T> &scale) noexcept
	{
		SetIdentity();

		m[0] = scale.v[0];
		m[4] = scale.v[1];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetScale(const Vector<3, T> &scale) noexcept
	{
		SetIdentity();

		m[0] = scale.v[0];
		m[5] = scale.v[1];
		m[10] = scale.v[2];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	void SetScale(const T x, const T y) noexcept
	{
		SetIdentity();

		m[0] = x;
		m[4] = y;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetScale(const T x, const T y, const T z) noexcept
	{
		SetIdentity();

		m[0] = x;
		m[5] = y;
		m[10] = z;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	void SetRotation(const T angle) noexcept
	{
		SetIdentity();

		const T c = std::cos(angle);
		const T s = std::sin(angle);

		m[0] = c;
		m[3] = -s;
		m[1] = s;
		m[4] = c;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetRotation(const Vector<3, T> &axis, T angle) noexcept
	{
		const T x = axis.v[0];
		const T y = axis.v[1];
		const T z = axis.v[2];

		// Make sure the input axis is normalized
		T n = x * x + y * y + z * z;
		if( n != T(1) )
		{
			// Not normalized
			n = std::sqrt(n);
			// Prevent divide too close to zero
			if( n > std::numeric_limits<T>::min() )
			{
				n = T(1) / n;
				x *= n;
				y *= n;
				z *= n;
			}
		}

		const T c = std::cos(angle);
		const T s = std::sin(angle);

		const T t = T(1) - c;
		const T tx = t * x;
		const T ty = t * y;
		const T tz = t * z;
		const T txy = tx * y;
		const T txz = tx * z;
		const T tyz = ty * z;
		const T sx = s * x;
		const T sy = s * y;
		const T sz = s * z;

		m[0] = c + tx * x;
		m[4] = txy - sz;
		m[8] = txz + sy;
		m[12] = T(0);

		m[1] = txy + sz;
		m[5] = c + ty * y;
		m[9] = tyz - sx;
		m[13] = T(0);

		m[2] = txz - sy;
		m[6] = tyz + sx;
		m[10] = c + tz * z;
		m[14] = T(0);

		m[3] = T(0);
		m[7] = T(0);
		m[11] = T(0);
		m[15] = T(1);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetRotation(const Quaternion<T> &rotation) noexcept
	{
		const T wx = rotation.v[3] * rotation.v[0];
		const T wy = rotation.v[3] * rotation.v[1];
		const T wz = rotation.v[3] * rotation.v[2];

		const T xx = rotation.v[0] * rotation.v[0];
		const T xy = rotation.v[0] * rotation.v[1];
		const T xz = rotation.v[0] * rotation.v[2];

		const T yy = rotation.v[1] * rotation.v[1];
		const T yz = rotation.v[1] * rotation.v[2];

		const T zz = rotation.v[2] * rotation.v[2];

		m[0] = T(1) - T(2) * (yy + zz);
		m[4] = T(2) * (xy - wz);
		m[8] = T(2) * (xz + wy);
		m[12] = T(0);

		m[1] = T(2) * (xy + wz);
		m[5] = T(1) - T(2) * (xx + zz);
		m[9] = T(2) * (yz - wx);
		m[13] = T(0);

		m[2] = T(2) * (xz - wy);
		m[6] = T(2) * (yz + wx);
		m[10] = T(1) - T(2) * (xx + yy);
		m[14] = T(0);

		m[3] = T(0);
		m[7] = T(0);
		m[11] = T(0);
		m[15] = T(1);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetRotationX(const T angle) noexcept
	{
		SetIdentity();

		const T c = std::cos(angle);
		const T s = std::sin(angle);

		m[5] = c;
		m[9] = -s;
		m[6] = s;
		m[10] = c;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetRotationY(const T angle) noexcept
	{
		SetIdentity();

		const T c = std::cos(angle);
		const T s = std::sin(angle);

		m[0] = c;
		m[8] = s;
		m[2] = -s;
		m[10] = c;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetRotationZ(const T angle) noexcept
	{
		SetIdentity();

		const T c = std::cos(angle);
		const T s = std::sin(angle);

		m[0] = c;
		m[4] = -s;
		m[1] = s;
		m[5] = c;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	void SetTranslation(const Vector<3, T> &translation) noexcept
	{
		SetIdentity();

		m[6] = translation.v[0];
		m[7] = translation.v[1];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetTranslation(const Vector<3, T> &translation) noexcept
	{
		SetIdentity();

		m[12] = translation.v[0];
		m[13] = translation.v[1];
		m[14] = translation.v[2];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	void SetTranslation(const T x, const T y) noexcept
	{
		SetIdentity();

		m[6] = x;
		m[7] = y;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void SetTranslation(const T x, const T y, const T z) noexcept
	{
		SetIdentity();

		m[12] = x;
		m[13] = y;
		m[14] = z;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetFrustumLeftPlane() const noexcept
	{
		return Plane<T>::MakeFrustumPlane(m[3] + m[0], m[7] + m[4], m[11] + m[8], m[15] + m[12]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetFrustumRightPlane() const noexcept
	{
		return Plane<T>::MakeFrustumPlane(m[3] - m[0], m[7] - m[4], m[11] - m[8], m[15] - m[12]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetFrustumBottomPlane() const noexcept
	{
		return Plane<T>::MakeFrustumPlane(m[3] + m[1], m[7] + m[5], m[11] + m[9], m[15] + m[13]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetFrustumTopPlane() const noexcept
	{
		return Plane<T>::MakeFrustumPlane(m[3] - m[1], m[7] - m[5], m[11] - m[9], m[15] - m[13]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetFrustumNearPlane() const noexcept
	{
		return Plane<T>::MakeFrustumPlane(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetFrustumFarPlane() const noexcept
	{
		return Plane<T>::MakeFrustumPlane(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	auto GetFrustum() const noexcept
	{
		return ConvexVolume<T>({
			GetFrustumLeftPlane(),
			GetFrustumRightPlane(),
			GetFrustumBottomPlane(),
			GetFrustumTopPlane(),
			GetFrustumNearPlane(),
			GetFrustumFarPlane()
			});
	}

	void Add(const T scalar) noexcept
	{
		Add(scalar, *this);
	}

	void Add(const T scalar, Matrix &dst) const noexcept
	{
		dst.m[0] = m[0] + scalar;
		dst.m[1] = m[1] + scalar;
		dst.m[2] = m[2] + scalar;
		dst.m[3] = m[3] + scalar;
		dst.m[4] = m[4] + scalar;
		dst.m[5] = m[5] + scalar;
		dst.m[6] = m[6] + scalar;
		dst.m[7] = m[7] + scalar;
		dst.m[8] = m[8] + scalar;
		dst.m[9] = m[9] + scalar;
		dst.m[10] = m[10] + scalar;
		dst.m[11] = m[11] + scalar;
		dst.m[12] = m[12] + scalar;
		dst.m[13] = m[13] + scalar;
		dst.m[14] = m[14] + scalar;
		dst.m[15] = m[15] + scalar;
	}

	void Add(const Matrix &matrix) noexcept
	{
		Add(matrix, *this);
	}

	void Add(const Matrix &matrix, Matrix &dst) const noexcept
	{
		dst.m[0] = m[0] + matrix.m[0];
		dst.m[1] = m[1] + matrix.m[1];
		dst.m[2] = m[2] + matrix.m[2];
		dst.m[3] = m[3] + matrix.m[3];
		dst.m[4] = m[4] + matrix.m[4];
		dst.m[5] = m[5] + matrix.m[5];
		dst.m[6] = m[6] + matrix.m[6];
		dst.m[7] = m[7] + matrix.m[7];
		dst.m[8] = m[8] + matrix.m[8];
		dst.m[9] = m[9] + matrix.m[9];
		dst.m[10] = m[10] + matrix.m[10];
		dst.m[11] = m[11] + matrix.m[11];
		dst.m[12] = m[12] + matrix.m[12];
		dst.m[13] = m[13] + matrix.m[13];
		dst.m[14] = m[14] + matrix.m[14];
		dst.m[15] = m[15] + matrix.m[15];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 1 && Y == 1)>::type* = nullptr>
	T Determinant() const noexcept
	{
		return m[0];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 2 && Y == 2)>::type* = nullptr>
	T Determinant() const noexcept
	{
		return m[0] * m[3] - m[1] * m[2];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	T Determinant() const noexcept
	{
		T a0 = m[0] * (m[4] * m[8] - m[5] * m[7]);
		T a1 = m[1] * (m[3] * m[8] - m[5] * m[6]);
		T a2 = m[2] * (m[3] * m[7] - m[4] * m[6]);
		return a0 - a1 + a2;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	T Determinant() const noexcept
	{
		T a0 = m[0] * m[5] - m[1] * m[4];
		T a1 = m[0] * m[6] - m[2] * m[4];
		T a2 = m[0] * m[7] - m[3] * m[4];
		T a3 = m[1] * m[6] - m[2] * m[5];
		T a4 = m[1] * m[7] - m[3] * m[5];
		T a5 = m[2] * m[7] - m[3] * m[6];
		T b0 = m[8] * m[13] - m[9] * m[12];
		T b1 = m[8] * m[14] - m[10] * m[12];
		T b2 = m[8] * m[15] - m[11] * m[12];
		T b3 = m[9] * m[14] - m[10] * m[13];
		T b4 = m[9] * m[15] - m[11] * m[13];
		T b5 = m[10] * m[15] - m[11] * m[14];
		return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetUpVector() const noexcept
	{
		return Vector<3, T>(m[4], m[5], m[6]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetDownVector() const noexcept
	{
		return Vector<3, T>(-m[4], -m[5], -m[6]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetLeftVector() const noexcept
	{
		return Vector<3, T>(-m[0], -m[1], -m[2]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetRightVector() const noexcept
	{
		return Vector<3, T>(m[0], m[1], m[2]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetForwardVector() const noexcept
	{
		return Vector<3, T>(-m[8], -m[9], -m[10]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	inline auto GetBackVector() const noexcept
	{
		return Vector<3, T>(m[8], m[9], m[10]);
	}

	void Invert() noexcept
	{
		Invert(*this);
	}

	void Invert(Matrix &dst) const noexcept
	{
		const T a0 = m[0] * m[5] - m[1] * m[4];
		const T a1 = m[0] * m[6] - m[2] * m[4];
		const T a2 = m[0] * m[7] - m[3] * m[4];
		const T a3 = m[1] * m[6] - m[2] * m[5];
		const T a4 = m[1] * m[7] - m[3] * m[5];
		const T a5 = m[2] * m[7] - m[3] * m[6];
		const T b0 = m[8] * m[13] - m[9] * m[12];
		const T b1 = m[8] * m[14] - m[10] * m[12];
		const T b2 = m[8] * m[15] - m[11] * m[12];
		const T b3 = m[9] * m[14] - m[10] * m[13];
		const T b4 = m[9] * m[15] - m[11] * m[13];
		const T b5 = m[10] * m[15] - m[11] * m[14];

		// Calculate the determinant
		const T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

		// Close to zero, can't invert
		if( std::fabs(det) <= std::numeric_limits<T>::min() ) return;

		Matrix inverse;
		inverse.m[0] = m[5] * b5 - m[6] * b4 + m[7] * b3;
		inverse.m[1] = -m[1] * b5 + m[2] * b4 - m[3] * b3;
		inverse.m[2] = m[13] * a5 - m[14] * a4 + m[15] * a3;
		inverse.m[3] = -m[9] * a5 + m[10] * a4 - m[11] * a3;

		inverse.m[4] = -m[4] * b5 + m[6] * b2 - m[7] * b1;
		inverse.m[5] = m[0] * b5 - m[2] * b2 + m[3] * b1;
		inverse.m[6] = -m[12] * a5 + m[14] * a2 - m[15] * a1;
		inverse.m[7] = m[8] * a5 - m[10] * a2 + m[11] * a1;

		inverse.m[8] = m[4] * b4 - m[5] * b2 + m[7] * b0;
		inverse.m[9] = -m[0] * b4 + m[1] * b2 - m[3] * b0;
		inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
		inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

		inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
		inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
		inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
		inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

		inverse.Multiply(T(1) / det, dst);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == Y)>::type* = nullptr>
	inline auto IsIdentity() const noexcept
	{
		for( size_t r = 0; r < R; ++r )
			for( size_t c = 0; c < C; ++c )
				if( m[r * C + c] != (r == c ? T(1) : T(0)) )
					return false;
		return true;
	}

	void Multiply(const T scalar) noexcept
	{
		Multiply(scalar, *this);
	}

	void Multiply(const T scalar, Matrix &dst) const noexcept
	{
		dst.m[0] = m[0] * scalar;
		dst.m[1] = m[1] * scalar;
		dst.m[2] = m[2] * scalar;
		dst.m[3] = m[3] * scalar;
		dst.m[4] = m[4] * scalar;
		dst.m[5] = m[5] * scalar;
		dst.m[6] = m[6] * scalar;
		dst.m[7] = m[7] * scalar;
		dst.m[8] = m[8] * scalar;
		dst.m[9] = m[9] * scalar;
		dst.m[10] = m[10] * scalar;
		dst.m[11] = m[11] * scalar;
		dst.m[12] = m[12] * scalar;
		dst.m[13] = m[13] * scalar;
		dst.m[14] = m[14] * scalar;
		dst.m[15] = m[15] * scalar;
	}

	void Multiply(const Matrix &matrix) noexcept
	{
		Multiply(matrix, *this);
	}

	void Multiply(const Matrix &matrix, Matrix &dst) const noexcept
	{
		const T product[16] = {
			m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3],
			m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3],
			m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3],
			m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3],

			m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7],
			m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7],
			m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7],
			m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7],

			m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11],
			m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11],
			m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11],
			m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11],

			m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15],
			m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15],
			m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15],
			m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15]
		};

		std::copy(std::begin(product), std::end(product), dst.m);
	}

	void Negate() noexcept
	{
		Negate(*this);
	}

	void Negate(Matrix &dst) const noexcept
	{
		dst.m[0] = -m[0];
		dst.m[1] = -m[1];
		dst.m[2] = -m[2];
		dst.m[3] = -m[3];
		dst.m[4] = -m[4];
		dst.m[5] = -m[5];
		dst.m[6] = -m[6];
		dst.m[7] = -m[7];
		dst.m[8] = -m[8];
		dst.m[9] = -m[9];
		dst.m[10] = -m[10];
		dst.m[11] = -m[11];
		dst.m[12] = -m[12];
		dst.m[13] = -m[13];
		dst.m[14] = -m[14];
		dst.m[15] = -m[15];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == Y)>::type* = nullptr>
	inline void SetIdentity() noexcept
	{
		for( size_t r = 0; r < R; ++r )
			for( size_t c = 0; c < C; ++c )
				m[r * C + c] = static_cast<T>(r == c ? T(1) : T(0));
	}

	inline void SetZero() noexcept
	{
		for( T& c : m )
			c = T(0);
	}

	void Subtract(const Matrix &matrix) noexcept
	{
		Subtract(matrix, *this);
	}

	void Subtract(const Matrix &matrix, Matrix &dst) const noexcept
	{
		dst.m[0] = m[0] - matrix.m[0];
		dst.m[1] = m[1] - matrix.m[1];
		dst.m[2] = m[2] - matrix.m[2];
		dst.m[3] = m[3] - matrix.m[3];
		dst.m[4] = m[4] - matrix.m[4];
		dst.m[5] = m[5] - matrix.m[5];
		dst.m[6] = m[6] - matrix.m[6];
		dst.m[7] = m[7] - matrix.m[7];
		dst.m[8] = m[8] - matrix.m[8];
		dst.m[9] = m[9] - matrix.m[9];
		dst.m[10] = m[10] - matrix.m[10];
		dst.m[11] = m[11] - matrix.m[11];
		dst.m[12] = m[12] - matrix.m[12];
		dst.m[13] = m[13] - matrix.m[13];
		dst.m[14] = m[14] - matrix.m[14];
		dst.m[15] = m[15] - matrix.m[15];
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void TransformPoint(Vector<3, T> &point) const noexcept
	{
		TransformVector(point.v[0], point.v[1], point.v[2], 1, point);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void TransformPoint(const Vector<3, T> &point, Vector<3, T> & dst) const noexcept
	{
		TransformVector(point.v[0], point.v[1], point.v[2], 1, dst);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void TransformVector(Vector<3, T> &v) const noexcept
	{
		Vector<4, T> t;
		TransformVector(Vector<4, T>(v.v[0], v.v[1], v.v[2], T(0)), t);
		v = Vector<3, T>(t.v[0], t.v[1], t.v[2]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void TransformVector(const Vector<3, T> &v, Vector<3, T> &dst) const noexcept
	{
		TransformVector(v.v[0], v.v[1], v.v[2], 0, dst);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void TransformVector(const T x, const T y, const T z, const T w, Vector<3, T> &dst) const noexcept
	{
		Vector<4, T> t;
		TransformVector(Vector<4, T>(x, y, z, w), t);
		dst = Vector<3, T>(t.v[0], t.v[1], t.v[2]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	void TransformVector(Vector<4, T> &v) const noexcept
	{
		TransformVector(v, v);
	}

	void TransformVector(const Vector<4, T> &v, Vector<4, T> &dst) const noexcept
	{
		assert(&v != &dst);
		dst.v[0] = v.v[0] * m[0] + v.v[1] * m[4] + v.v[2] * m[8] + v.v[3] * m[12];
		dst.v[1] = v.v[0] * m[1] + v.v[1] * m[5] + v.v[2] * m[9] + v.v[3] * m[13];
		dst.v[2] = v.v[0] * m[2] + v.v[1] * m[6] + v.v[2] * m[10] + v.v[3] * m[14];
		dst.v[3] = v.v[0] * m[3] + v.v[1] * m[7] + v.v[2] * m[11] + v.v[3] * m[15];
	}

	void Transpose() noexcept
	{
		Transpose(*this);
	}

	void Transpose(Matrix &dst) const noexcept
	{
		const T t[16] =
		{
			m[0], m[4], m[8], m[12],
			m[1], m[5], m[9], m[13],
			m[2], m[6], m[10], m[14],
			m[3], m[7], m[11], m[15]
		};
		std::copy(std::begin(t), std::end(t), dst.m);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	Vector<2, T> GetTranslation() const noexcept
	{
		return Vector<2, T>(m[6], m[7]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	Vector<3, T> GetTranslation() const noexcept
	{
		return Vector<3, T>(m[12], m[13], m[14]);
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 3 && Y == 3)>::type* = nullptr>
	Vector<2, T> GetScale() const noexcept
	{
		Vector<2, T> scale;
		scale.v[0] = Vector<2, T>(m[0], m[1]).Length();
		scale.v[1] = Vector<2, T>(m[3], m[4]).Length();

		return scale;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	Vector<3, T> GetScale() const noexcept
	{
		Vector<3, T> scale;
		scale.v[0] = Vector<3, T>(m[0], m[1], m[2]).Length();
		scale.v[1] = Vector<3, T>(m[4], m[5], m[6]).Length();
		scale.v[2] = Vector<3, T>(m[8], m[9], m[10]).Length();

		return scale;
	}

	template <size_t X = C, size_t Y = R, typename std::enable_if<(X == 4 && Y == 4)>::type* = nullptr>
	auto GetRotation() const noexcept
	{
		const Vector<3, T> scale = GetScale();

		const T m11 = m[0] / scale.v[0];
		const T m21 = m[1] / scale.v[0];
		const T m31 = m[2] / scale.v[0];

		const T m12 = m[4] / scale.v[1];
		const T m22 = m[5] / scale.v[1];
		const T m32 = m[6] / scale.v[1];

		const T m13 = m[8] / scale.v[2];
		const T m23 = m[9] / scale.v[2];
		const T m33 = m[10] / scale.v[2];

		Quaternion<T> result;
		result.v[0] = std::sqrt(std::max(static_cast<T>(0), T(1) + m11 - m22 - m33)) / T(2);
		result.v[1] = std::sqrt(std::max(static_cast<T>(0), T(1) - m11 + m22 - m33)) / T(2);
		result.v[2] = std::sqrt(std::max(static_cast<T>(0), T(1) - m11 - m22 + m33)) / T(2);
		result.v[3] = std::sqrt(std::max(static_cast<T>(0), T(1) + m11 + m22 + m33)) / T(2);

		// The problem with using copysign: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/paul.htm
		result.v[0] = std::copysign(result.v[0], m32 - m23);
		result.v[1] = std::copysign(result.v[1], m13 - m31);
		result.v[2] = std::copysign(result.v[2], m21 - m12);

		result.Normalize();

		return result;
	}

#if defined(__SSE__)
	alignas((C == 4 && R == 4) ? 4 * sizeof(T) : alignof(T))
#endif
	T m[C * R]{ 0 };
};

template <typename T>
inline Vector<3, T>& operator*=(Vector<3, T> &v, const Matrix<4, 4, T> &m) noexcept
{
	m.TransformVector(v);
	return v;
}

template <typename T>
inline Vector<4, T>& operator*=(Vector<4, T> &v, const Matrix<4, 4, T> &m) noexcept
{
	m.TransformVector(v);
	return v;
}

using Matrix4F = Matrix<4, 4, float>;