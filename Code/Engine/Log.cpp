#include "stdafx.h"
#include "Log.h"
#include "Console.h"
#include "OSPlatformUtils.h"
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

	if (Console::IsValid() && m_config.PrintConsole)
	{
		static auto &console = GetSubsystem<Console>();
		console.Print(str);
	}		

	if (OSPlatformUtils::IsValid() && m_config.PrintDebugOutput)
	{
		static auto &osPlatform = GetSubsystem<OSPlatformUtils>();
		osPlatform.PrintDebugOutput(str);
	}		

	TODO("����� � ���������� ����");
	TODO("������� � ����� ����� ���� �����������?");
	TODO("� ������ ��� ������ ����������� ������ � �� ����� ��������");
}
//-----------------------------------------------------------------------------