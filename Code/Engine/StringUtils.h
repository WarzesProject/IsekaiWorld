#pragma once

// Returns the specified integral value as hexadecimal string.
template <typename T>
std::string ToHex(T value)
{
	static_assert(std::is_integral<T>::value, "input parameter of 'LLGL::ToHex' must be an integral type");
	std::stringstream s;
	s << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << std::uppercase << static_cast<std::uint64_t>(value);
	return s.str();
}