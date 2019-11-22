#pragma once

#include "LogConfig.h"
#include "Log.h"
#include "Subsystem.h"
#include "Thread.h"

class Logger final : public Subsystem<Logger>
{
	friend Log;
#if !SE_PLATFORM_EMSCRIPTEN
	struct Command
	{
		enum class Type
		{
			LogString,
			Quit
		};

		explicit Command(const Type initType) : type(initType) {}
		Command(const Type initType, const Log::Level initLevel, const std::string &initString) : type(initType), level(initLevel), str(initString) {}

		Type type;
		Log::Level level;
		std::string str;
	};
#endif

public:
	Logger(LogConfig &config, Log::Level initThreshold = Log::Level::All);
	~Logger();

	Log CreateLog(Log::Level level) const { return Log(*this, level); }

	void Error(std::string_view str);
	void Warning(std::string_view str);
	void Info(std::string_view str);
	void Debug(std::string_view str);

private:
	void print(const std::string &str, const Log::Level level = Log::Level::Info) const;
	void printString(const std::string &str, const Log::Level level = Log::Level::Info);

	LogConfig &m_config;

#if SE_DEBUG
	std::atomic<Log::Level> m_threshold{ Log::Level::All };
#else
	std::atomic<Log::Level> m_threshold{ Log::Level::Info };
#endif

#if !SE_PLATFORM_EMSCRIPTEN
	void logLoop();

	mutable std::condition_variable m_queueCondition;
	mutable std::mutex m_queueMutex;
	mutable std::queue<Command> m_commandQueue;
	Thread m_logThread;
#endif
};