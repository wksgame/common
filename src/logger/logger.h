#ifndef KISS_LOGGER_H
#define KISS_LOGGER_H

#include<stdio.h>
#include<string.h>
#include"thread/Mutex.h"
#include"defines.h"

namespace kiss
{
	enum class LogLevel
	{
		error=0,
		warn,
		info,
		debug,
		end
	};

	class Logger
	{
		std::string file_name;
		int64 log_day_usec;
		Mutex<true> logger_mutex;
		FILE* file_fd;

	public:
		Logger(const char* log_file_name);
		~Logger();

		void logger(const kiss::LogLevel level, const char* format, ...);
	};//Logger

	extern Logger syslogger;

#ifdef LOG_OUTPUT
#	define LOG_ERROR(str,...)		syslogger.logger(kiss::LogLevel::error, str, ##__VA_ARGS__)
#	define LOG_WARN(str,...)		syslogger.logger(LogLevel::warn, str, ##__VA_ARGS__)
#	define LOG_INFO(str,...)		syslogger.logger(LogLevel::info, str, ##__VA_ARGS__)
#	define LOG_DEBUG(str,...)		syslogger.logger(LogLevel::debug, str, ##__VA_ARGS__)
#else
#	define LOG_ERROR(str,...)
#	define LOG_WARN(str,...)
#	define LOG_INFO(str,...)
#	define LOG_DEBUG(str,...)
#endif//LOG_OUTPUT
}//namespace kiss

#endif//KISS_LOGGER_H