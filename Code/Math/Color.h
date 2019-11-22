#pragma once

#include "Vector.h"

constexpr uint8_t HexToInt(const char c)
{
	return (c >= '0' && c <= '9') ? static_cast<uint8_t>(c - '0') :
		(c >= 'a' && c <= 'f') ? static_cast<uint8_t>(c - 'a' + 10) :
		(c >= 'A' && c <= 'F') ? static_cast<uint8_t>(c - 'A' + 10) :
		throw std::out_of_range("Invalid hex digit");
}

class Color final
{
public:
	constexpr Color() noexcept {}

	constexpr Color(uint32_t color) noexcept :
		v{
			static_cast<uint8_t>((color & 0xFF000000) >> 24),
			static_cast<uint8_t>((color & 0x00FF0000) >> 16),
			static_cast<uint8_t>((color & 0x0000FF00) >> 8),
			static_cast<uint8_t>(color & 0x000000FF)
		}
	{
	}

	Color(const std::string &color)
	{
		if( !color.empty() )
		{
			if( color.front() == '#' )
			{
				assert(color.length() == 4 || color.length() == 7);

				const size_t componentSize = (color.length() - 1) / 3; // exclude the #

				for( size_t component = 0; component < 3; ++component )
				{
					v[component] = 0;

					for( size_t byte = 0; byte < 2; ++byte )
					{
						const char c = (byte < componentSize) ? color[component * componentSize + byte + 1] : color[component * componentSize + 1];
						v[component] = static_cast<uint8_t>((v[component] << 4) | HexToInt(c));
					}
				}

				v[3] = 0xFF; // alpha
			}
			else
			{
				const auto intValue = static_cast<uint32_t>(std::stoul(color));
				v[0] = static_cast<uint8_t>((intValue & 0xFF000000) >> 24);
				v[1] = static_cast<uint8_t>((intValue & 0x00FF0000) >> 16);
				v[2] = static_cast<uint8_t>((intValue & 0x0000FF00) >> 8);
				v[3] = static_cast<uint8_t>(intValue & 0x000000FF);
			}
		}
		else
			for( size_t i = 0; i < 4; ++i )
				v[i] = 0;
	}

	Color(const char *color) : Color(std::string(color)) {}

	constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) noexcept : v{ red, green, blue, alpha } {}

	explicit Color(float color[4]) noexcept :
		v{
			static_cast<uint8_t>(std::round(color[0] * 255.0F)),
			static_cast<uint8_t>(std::round(color[1] * 255.0F)),
			static_cast<uint8_t>(std::round(color[2] * 255.0F)),
			static_cast<uint8_t>(std::round(color[3] * 255.0F))
		}
	{
	}

	explicit Color(const Vector<3, float> &vec) noexcept :
		v{
			static_cast<uint8_t>(std::round(vec.v[0] * 255.0F)),
			static_cast<uint8_t>(std::round(vec.v[1] * 255.0F)),
			static_cast<uint8_t>(std::round(vec.v[2] * 255.0F)),
			0
		}
	{
	}

	explicit Color(const Vector<4, float> &vec) noexcept :
		v{
			static_cast<uint8_t>(std::round(vec.v[0] * 255.0F)),
			static_cast<uint8_t>(std::round(vec.v[1] * 255.0F)),
			static_cast<uint8_t>(std::round(vec.v[2] * 255.0F)),
			static_cast<uint8_t>(std::round(vec.v[3] * 255.0F))
	}
	{
	}

	constexpr bool operator<(const Color &c) const noexcept
	{
		return v[0] == c.v[0] ?
			v[1] == c.v[1] ?
			v[2] == c.v[2] ?
			v[3] == c.v[3] ?
			false :
			v[3] < c.v[3] :
			v[2] < c.v[2] :
			v[1] < c.v[1] :
			v[0] < c.v[0];
	}

	constexpr bool operator==(const Color &c) const noexcept
	{
		return v[0] == c.v[0] &&
			v[1] == c.v[1] &&
			v[2] == c.v[2] &&
			v[3] == c.v[3];
	}

	constexpr bool operator!=(const Color &c) const noexcept
	{
		return v[0] != c.v[0] ||
			v[1] != c.v[1] ||
			v[2] != c.v[2] ||
			v[3] != c.v[3];
	}

	static constexpr Color Black() noexcept { return Color(0, 0, 0, 255); }
	static constexpr Color Red() noexcept { return Color(255, 0, 0, 255); }
	static constexpr Color Magenta() noexcept { return Color(255, 0, 255, 255); }
	static constexpr Color Green() noexcept { return Color(0, 255, 0, 255); }
	static constexpr Color Cyan() noexcept { return Color(0, 255, 255, 255); }
	static constexpr Color Blue() noexcept { return Color(0, 0, 255, 255); }
	static constexpr Color Yellow() noexcept { return Color(255, 255, 0, 255); }
	static constexpr Color White() noexcept { return Color(255, 255, 255, 255); }
	static constexpr Color Gray() noexcept { return Color(128, 128, 128, 255); }

	inline uint8_t& operator[](size_t index) noexcept { return v[index]; }
	constexpr uint8_t operator[](size_t index) const noexcept { return v[index]; }

	inline uint8_t& r() noexcept { return v[0]; }
	constexpr uint8_t r() const noexcept { return v[0]; }

	inline uint8_t& g() noexcept { return v[1]; }
	constexpr uint8_t g() const noexcept { return v[1]; }

	inline uint8_t& b() noexcept { return v[2]; }
	constexpr uint8_t b() const noexcept { return v[2]; }

	inline uint8_t& a() noexcept { return v[3]; }
	constexpr uint8_t a() const noexcept { return v[3]; }

	constexpr std::array<float, 4> Norm() const noexcept
	{
		return { {v[0] / 255.0F, v[1] / 255.0F, v[2] / 255.0F, v[3] / 255.0F} };
	}
	constexpr float NormR() const noexcept { return v[0] / 255.0F; }
	constexpr float NormG() const noexcept { return v[1] / 255.0F; }
	constexpr float NormB() const noexcept { return v[2] / 255.0F; }
	constexpr float NormA() const noexcept { return v[3] / 255.0F; }

	constexpr auto GetIntValue() const noexcept
	{
		return (static_cast<uint32_t>(v[0]) << 24) |
			(static_cast<uint32_t>(v[1]) << 16) |
			(static_cast<uint32_t>(v[2]) << 8) |
			static_cast<uint32_t>(v[3]);
	}

	constexpr auto IsZero() const noexcept
	{
		return v[0] == 0 &&
			v[1] == 0 &&
			v[2] == 0 &&
			v[3] == 0;
	}

	uint8_t v[4]{ 0 };
};