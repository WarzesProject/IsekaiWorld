#pragma once

#if defined(__ARM_NEON__)
#  if defined(__ANDROID__) && defined(__arm__)
// NEON support must be checked at runtime on 32-bit Android
class AnrdoidNeonChecker final
{
public:
	AnrdoidNeonChecker() noexcept :
		neonAvailable(android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
		(android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0)
	{
	}

	operator bool() const noexcept { return neonAvailable; }

private:
	bool neonAvailable;
};

extern const AnrdoidNeonChecker isSimdAvailable;
#  else
constexpr auto IsSimdAvailable = true;
#  endif
#elif defined(__SSE__)
constexpr auto IsSimdAvailable = true;
#else
constexpr auto IsSimdAvailable = false;
#endif

template <typename T>
constexpr T Lerp(const T v0, const T v1, const T t) noexcept
{
	return (T(1) - t) * v0 + t * v1;
}

template <typename T>
constexpr T SmoothStep(const T a, const T b, const T t) noexcept
{
	return Lerp(a, b, t * t * (T(3) - T(2) * t));
}

template <typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
constexpr auto IsPowerOfTwo(const T x) noexcept
{
	return (x != T(0)) && (((x - T(1)) & x) == 0);
}

template <typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
inline T NextPowerOfTwo(const T x) noexcept
{
	if( x != 0 )
	{
		--x;
		for( uint32_t shift = 1; shift < sizeof(T) * 8; shift *= 2 )
			x |= (x >> shift);
	}
	return ++x;
}

template <typename T> constexpr T DegToRad(const T x) noexcept
{
	return x * T(0.01745329251994329576);
}

template <typename T> constexpr T RadToDeg(const T x) noexcept
{
	return x * T(57.2957795130823208767);
}

template <typename T>
constexpr T Clamp(const T x, const T lo, const T hi) noexcept
{
	return (x < lo) ? lo : ((x > hi) ? hi : x);
}

template <typename T>
constexpr auto IsNearlyEqual(const T a, const T b, const T tolerance = std::numeric_limits<T>::min()) noexcept
{
	return (a - b) <= tolerance && (a - b) >= -tolerance;
}

uint32_t AddUInt32Clamped(uint32_t lhs, uint32_t rhs);
uint32_t SubUInt32Clamped(uint32_t lhs, uint32_t rhs);

int32_t AddInt32Clamped(int32_t lhs, int32_t rhs);
int32_t SubInt32Clamped(int32_t lhs, int32_t rhs);

template <typename T> constexpr T tau = T(6.28318530717958647692);
template <typename T> constexpr T pi = T(3.14159265358979323846);

namespace fnv
{
	template <typename T> T prime;
	template <typename T> T offsetBasis;

	template <> constexpr uint32_t prime<uint32_t> = 16777619u;
	template <> constexpr uint32_t offsetBasis<uint32_t> = 2166136261u;
	template <> constexpr uint64_t prime<uint64_t> = 1099511628211u;
	template <> constexpr uint64_t offsetBasis<uint64_t> = 14695981039346656037u;

	// Fowler / Noll / Vo (FNV) hash
	template <typename Result, typename Value>
	constexpr Result hash(const Value value, size_t i = 0, Result result = offsetBasis<Result>) noexcept
	{
		return (i < sizeof(Value)) ? hash<Result>(value, i + 1, (result * prime<Result>) ^ ((value >> (i * 8)) & 0xFF)) : result;
	}
} // namespace fnv