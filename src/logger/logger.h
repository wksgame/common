#ifndef KISS_LOGGER_H
#define KISS_LOGGER_H

#include<string>

namespace kiss
{
	enum class LogLevel
	{
		error=0,
		warn,
		hint,
		normal,
	};

	void logger(const LogLevel level, const char* format,...);
	void logger(const LogLevel level, const std::string& text);

#ifndef LOG_NOT_OUTPUT
#	define LOG_ERROR(str,...)		logger(LogLevel::error, str, ##__VA_ARGS__)
#	define LOG_WARN(str,...)		logger(LogLevel::warn, str, ##__VA_ARGS__)
#	define LOG_HINT(str,...)		logger(LogLevel::hint, str, ##__VA_ARGS__)
#	define LOG_INFO(str,...)		logger(LogLevel::normal, str, ##__VA_ARGS__)
#else
#	define LOG_ERROR(str,...)
#	define LOG_WARN(str,...)
#	define LOG_HINT(str,...)
#	define LOG_INFO(str,...)
#endif//LOG_OUTPUT
}//namespace kiss

#endif//KISS_LOGGER_H