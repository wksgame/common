#ifndef KISS_LOGGER_H
#define KISS_LOGGER_H

#include"thread/Mutex.h"
#include<stdio.h>

namespace kiss
{
	enum class LogLevel
	{
		error=0,
		warn,
		hint,
		normal,
		end
	};

	class Logger
	{
		const char* file_name;
		long long int cur_time;
		long long int log_day_msec;
		long long int write_time;					// write to disk
		Mutex<true> logger_mutex;
		FILE* file_fd;

		void logger(const kiss::LogLevel level, const char* format, va_list& args);

	public:
		Logger(const char* log_file_name);
		~Logger();

		void error(const char* format,...);
		void warn(const char* format,...);
		void hint(const char* format,...);
		void info(const char* format,...);

	};//Logger

	extern Logger syslogger;

#ifdef LOG_OUTPUT
#	define LOG_ERROR(str,...)		syslogger.error(str, ##__VA_ARGS__)
#	define LOG_WARN(str,...)		syslogger.warn(str, ##__VA_ARGS__)
#	define LOG_HINT(str,...)		syslogger.hint(str, ##__VA_ARGS__)
#	define LOG_INFO(str,...)		syslogger.info(str, ##__VA_ARGS__)
#else
#	define LOG_ERROR(str,...)
#	define LOG_WARN(str,...)
#	define LOG_HINT(str,...)
#	define LOG_INFO(str,...)
#endif//LOG_OUTPUT
}//namespace kiss

#endif//KISS_LOGGER_H