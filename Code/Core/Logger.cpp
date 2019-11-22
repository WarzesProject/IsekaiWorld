#include "stdafx.h"
#include "Logger.h"
TODO("исправить зависимости")
#include "Platform/Console.h"
#include "Platform/Platform.h"

//-----------------------------------------------------------------------------
Log::~Log()
{
	if( !m_str.empty() )
		m_logger.print(m_str, m_level);
}
//-----------------------------------------------------------------------------
Logger::Logger(LogConfig &config, Log::Level initThreshold)
	: m_config(config)
	, m_threshold(initThreshold)
{
#if !SE_PLATFORM_EMSCRIPTEN
	m_logThread = Thread(&Logger::logLoop, this);
#endif
}
//-----------------------------------------------------------------------------
Logger::~Logger()
{
#if !SE_PLATFORM_EMSCRIPTEN
	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_commandQueue.push(Command(Command::Type::Quit));
	lock.unlock();
	m_queueCondition.notify_all();
#endif
}
//-----------------------------------------------------------------------------
void Logger::print(const std::string &str, const Log::Level level = Log::Level::Info) const
{
	if( level <= m_threshold )
	{
#if SE_PLATFORM_EMSCRIPTEN
		printString(str, level);
#else
		std::unique_lock<std::mutex> lock(m_queueMutex);
		m_commandQueue.push(Command(Command::Type::LogString, level, str));
		lock.unlock();
		m_queueCondition.notify_all();
#endif
	}
}
//-----------------------------------------------------------------------------
#if !SE_PLATFORM_EMSCRIPTEN
void Logger::logLoop()
{
	for( ;;)
	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
		while( m_commandQueue.empty() ) m_queueCondition.wait(lock);
		auto command = std::move(m_commandQueue.front());
		m_commandQueue.pop();
		lock.unlock();

		if( command.type == Command::Type::LogString )
			printString(command.str, command.level);
		else if( command.type == Command::Type::Quit )
			break;
	}
}
#endif
//-----------------------------------------------------------------------------
void Logger::printString(const std::string &str, Log::Level level)
{
#if SE_PLATFORM_ANDROID
	int priority = 0;
	switch( level )
	{
	case Log::Level::Error: priority = ANDROID_LOG_ERROR; break;
	case Log::Level::Warning: priority = ANDROID_LOG_WARN; break;
	case Log::Level::Info: priority = ANDROID_LOG_INFO; break;
	case Log::Level::All: priority = ANDROID_LOG_DEBUG; break;
	default: return;
	}
	__android_log_print(priority, "Sapphire", "%s", str.c_str());
#elif SE_PLATFORM_LINUX
	int fd = 0;
	switch( level )
	{
	case Log::Level::Error:
	case Log::Level::Warning:
		fd = STDERR_FILENO;
		break;
	case Log::Level::Info:
	case Log::Level::All:
		fd = STDOUT_FILENO;
		break;
	default: return;
	}

	std::vector<char> output(str.begin(), str.end());
	output.push_back('\n');

	size_t offset = 0;
	while( offset < output.size() )
	{
		ssize_t written = write(fd, output.data() + offset, output.size() - offset);
		if( written == -1 )
			return;

		offset += static_cast<size_t>(written);
	}
#elif SE_PLATFORM_WINDOWS
	const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if( bufferSize == 0 )
		return;

	std::vector<WCHAR> buffer(bufferSize + 1); // +1 for the newline
	if( MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), static_cast<int>(buffer.size())) == 0 )
		return;

	if( FAILED(StringCchCatW(buffer.data(), buffer.size(), L"\n")) )
		return;

	OutputDebugStringW(buffer.data());
#	if SE_DEBUG
	HANDLE handle;
	switch( level )
	{
	case Log::Level::Error:
	case Log::Level::Warning:
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	case Log::Level::Info:
	case Log::Level::All:
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	default: return;
	}

	DWORD bytesWritten;
	WriteConsoleW(handle, buffer.data(), static_cast<DWORD>(wcslen(buffer.data())), &bytesWritten, nullptr);
#	endif
#elif SE_PLATFORM_EMSCRIPTEN
	int flags = EM_LOG_CONSOLE;
	switch( level )
	{
	case Log::Level::Error:
		flags |= EM_LOG_ERROR;
		break;
	case Log::Level::Warning:
		flags |= EM_LOG_WARN;
		break;
	case Log::Level::Info:
	case Log::Level::All:
		break;
	default: return;
	}
	emscripten_log(flags, "%s", str.c_str());
#endif
}









//-----------------------------------------------------------------------------
Logs::Logs(LogConfig &config)
	: m_config(config)
{
	setValid(true);
}
//-----------------------------------------------------------------------------
void Logs::Error(std::string_view str)
{
	if (!Logs::IsValid()) return;
	static auto &log = GetSubsystem<Logs>();
	log.print(Log::Level::Error, str);
}
//-----------------------------------------------------------------------------
void Logs::Warning(std::string_view str)
{
	if (!Logs::IsValid()) return;
	static auto &log = GetSubsystem<Logs>();
	log.print(Log::Level::Warning, str);
}
//-----------------------------------------------------------------------------
void Logs::Info(std::string_view str)
{
	if (!Logs::IsValid()) return;
	static auto &log = GetSubsystem<Logs>();
	log.print(Log::Level::Info, str);
}
//-----------------------------------------------------------------------------
void Logs::Debug(std::string_view str)
{
	if (!Logs::IsValid()) return;
	static auto &log = GetSubsystem<Logs>();
	log.print(Log::Level::Debug, str);
}
//-----------------------------------------------------------------------------
void Logs::print(Log::Level type, std::string_view str)
{
#if !SE_DEBUG
	if (type == LogType::Debug)
		return;
#endif

	if ( m_config.PrintConsole && Console::IsValid() )
	{
		static auto &console = GetSubsystem<Console>();
		console.Print(str);
	}		

	if (m_config.PrintDebugOutput && Platform::IsValid() )
	{
		static auto &osPlatform = GetSubsystem<Platform>();
		osPlatform.PrintDebugOutput(str);
	}		

	TODO("вывод в слушателей лога");
	TODO("консоль и дебаг могут быть слушателями?");
	TODO("а вообще лог должен накапливать записи а не сразу выводить");
}
//-----------------------------------------------------------------------------