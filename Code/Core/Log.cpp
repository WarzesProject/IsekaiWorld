#include "stdafx.h"
#include "Log.h"
TODO("исправить зависимости")
#include "Platform/Console.h"
#include "Platform/Platform.h"
//-----------------------------------------------------------------------------
Log::Log(LogConfig &config)
	: m_config(config)
{
	setValid(true);
}
//-----------------------------------------------------------------------------
void Log::Error(std::string_view str)
{
	if (!Log::IsValid()) return;
	static auto &log = GetSubsystem<Log>();
	log.print(LogType::Error, str);
}
//-----------------------------------------------------------------------------
void Log::Warning(std::string_view str)
{
	if (!Log::IsValid()) return;
	static auto &log = GetSubsystem<Log>();
	log.print(LogType::Warning, str);
}
//-----------------------------------------------------------------------------
void Log::Info(std::string_view str)
{
	if (!Log::IsValid()) return;
	static auto &log = GetSubsystem<Log>();
	log.print(LogType::Info, str);
}
//-----------------------------------------------------------------------------
void Log::Debug(std::string_view str)
{
	if (!Log::IsValid()) return;
	static auto &log = GetSubsystem<Log>();
	log.print(LogType::Debug, str);
}
//-----------------------------------------------------------------------------
void Log::print(LogType type, std::string_view str)
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