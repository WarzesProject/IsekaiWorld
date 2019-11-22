#pragma once

class Logger;

class Log final
{
public:
	enum class Level
	{
		Off,
		Error,
		Warning,
		Info,
		Debug,
		All
	};


	explicit Log(const Logger &initLogger, Level initLevel = Level::Info) : m_logger(initLogger), m_level(initLevel) {}
	Log(const Log &other) : m_logger(other.m_logger), m_level(other.m_level), m_str(other.m_str) {}

	Log(Log &&other) noexcept : m_logger(other.m_logger), m_level(other.m_level), m_str(std::move(other.m_str))
	{
		other.m_level = Level::Info;
	}

	Log& operator=(const Log &other)
	{
		if( &other == this ) return *this;

		m_level = other.m_level;
		m_str = other.m_str;

		return *this;
	}

	Log& operator=(Log &&other) noexcept
	{
		if( &other == this ) return *this;

		m_level = other.m_level;
		other.m_level = Level::Info;
		m_str = std::move(other.m_str);

		return *this;
	}

	~Log();

	template <typename T, typename std::enable_if<std::is_same<T, bool>::value>::type* = nullptr>
	Log & operator<<(const T val)
	{
		m_str += val ? "true" : "false";
		return *this;
	}

	template <typename T, typename std::enable_if<std::is_same<T, uint8_t>::value>::type* = nullptr>
	Log & operator<<(const T val)
	{
		constexpr char digits[] = "0123456789abcdef";

		for( uint32_t p = 0; p < 2; ++p )
			m_str.push_back(digits[(val >> (4 - p * 4)) & 0x0F]);

		return *this;
	}

	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value &&
		!std::is_same<T, bool>::value &&
		!std::is_same<T, uint8_t>::value>::type* = nullptr>
		Log & operator<<(const T val)
	{
		m_str += std::to_string(val);
		return *this;
	}

	template <typename T, typename std::enable_if<std::is_same<T, std::string>::value>::type* = nullptr>
	Log & operator<<(const T & val)
	{
		m_str += val;
		return *this;
	}

	template <typename T, typename std::enable_if<std::is_same<T, char>::value>::type* = nullptr>
	Log & operator<<(const T *val)
	{
		m_str += val;
		return *this;
	}

	template <typename T, typename std::enable_if<!std::is_same<T, char>::value>::type* = nullptr>
	Log & operator<<(const T *val)
	{
		constexpr char digits[] = "0123456789abcdef";

		uintptr_t ptrValue;
		memcpy(&ptrValue, &val, sizeof(ptrValue));

		for( size_t i = 0; i < sizeof(val) * 2; ++i )
			m_str.push_back(digits[(ptrValue >> (sizeof(ptrValue) * 2 - i - 1) * 4) & 0x0F]);

		return *this;
	}

	template <typename T> struct isContainer : std::false_type {};
	template <typename T, std::size_t N> struct isContainer<std::array<T, N>> : std::true_type {};
	template <typename... Args> struct isContainer<std::initializer_list<Args...>> : std::true_type {};
	template <typename... Args> struct isContainer<std::list<Args...>> : std::true_type {};
	template <typename... Args> struct isContainer<std::set<Args...>> : std::true_type {};
	template <typename... Args> struct isContainer<std::vector<Args...>> : std::true_type {};

	template <typename T, typename std::enable_if<isContainer<T>::value>::type* = nullptr>
	Log & operator<<(const T &val)
	{
		bool first = true;

		for( const auto &i : val )
		{
			if( !first ) m_str += ", ";
			first = false;

			operator<<(i);
		}

		return *this;
	}

private:
	const Logger &m_logger;
	Level m_level = Level::Info;
	std::string m_str;
};