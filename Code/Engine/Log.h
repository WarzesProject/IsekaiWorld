#pragma once

#include "Module.h"
#include "LogConfig.h"

enum class LogType
{
	Error,
	Warning,
	Info,
	Debug
};

class Log : public Module<Log>
{
public:
	Log(LogConfig &config);

	void Print(LogType type, std::string_view str);

private:
	LogConfig &m_config;
};
