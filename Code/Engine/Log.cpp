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
void Log::Print(LogType type, std::string_view str)
{
#if !SE_DEBUG
	if (type == LogType::Debug)
		return;
#endif

	if (Console::IsValid() && m_config.PrintConsole)
		GetModule<Console>().Print(str);

	if (OSPlatformUtils::IsValid() && m_config.PrintDebugOutput)
		GetModule<OSPlatformUtils>().PrintDebugOutput(str);

	TODO("вывод в слушателей лога");
	TODO("консоль и дебаг могут быть слушателями?");
	TODO("а вообще лог должен накапливать записи а не сразу выводить");
}
//-----------------------------------------------------------------------------