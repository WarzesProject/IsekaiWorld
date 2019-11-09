#include "stdafx.h"
#include "OldLog.h"
TODO("исправить зависимости")
#include "Platform/Console.h"
#include "Platform/Platform.h"
//-----------------------------------------------------------------------------
OldLog::OldLog(LogConfig &config)
	: m_config(config)
{
	setValid(true);
}
//-----------------------------------------------------------------------------
void OldLog::Error(std::string_view str)
{
	if (!OldLog::IsValid()) return;
	static auto &log = GetSubsystem<OldLog>();
	log.print(LogType::Error, str);
}
//-----------------------------------------------------------------------------
void OldLog::Warning(std::string_view str)
{
	if (!OldLog::IsValid()) return;
	static auto &log = GetSubsystem<OldLog>();
	log.print(LogType::Warning, str);
}
//-----------------------------------------------------------------------------
void OldLog::Info(std::string_view str)
{
	if (!OldLog::IsValid()) return;
	static auto &log = GetSubsystem<OldLog>();
	log.print(LogType::Info, str);
}
//-----------------------------------------------------------------------------
void OldLog::Debug(std::string_view str)
{
	if (!OldLog::IsValid()) return;
	static auto &log = GetSubsystem<OldLog>();
	log.print(LogType::Debug, str);
}
//-----------------------------------------------------------------------------
void OldLog::print(LogType type, std::string_view str)
{
#if !SE_DEBUG
	if (type == LogType::Debug)
		return;
#endif

	if (Console::IsValid() && m_config.PrintConsole)
	{
		static auto &console = GetSubsystem<Console>();
		console.Print(str);
	}		

	if (Platform::IsValid() && m_config.PrintDebugOutput)
	{
		static auto &osPlatform = GetSubsystem<Platform>();
		osPlatform.PrintDebugOutput(str);
	}		

	TODO("вывод в слушателей лога");
	TODO("консоль и дебаг могут быть слушателями?");
	TODO("а вообще лог должен накапливать записи а не сразу выводить");
}
//-----------------------------------------------------------------------------