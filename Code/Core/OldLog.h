#pragma once

#include "Subsystem.h"
#include "LogConfig.h"

enum class LogType
{
	Error,
	Warning,
	Info,
	Debug
};

class OldLog : public Subsystem<OldLog>
{
public:
	OldLog(LogConfig &config);

	static void Error(std::string_view str);	
	static void Warning(std::string_view str);
	static void Info(std::string_view str);
	static void Debug(std::string_view str);

private:
	void print(LogType type, std::string_view str);


	LogConfig &m_config;
};
