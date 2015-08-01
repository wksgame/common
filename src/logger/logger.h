#ifndef KISS_LOGGER_H
#define KISS_LOGGER_H

namespace kiss
{
	extern thread_local char thread_name[64];
	extern thread_local int thread_id;

	enum class LogLevel
	{
		error=0,
		warning,
		hint,
		normal,
	};

	void logger(const LogLevel level, const char* text);

#ifndef LOG_NOT_OUTPUT
#	define LOG_ERROR(str)		{logger(LogLevel::error, str);}
#	define LOG_WARNING(str)		{logger(LogLevel::warning, str);}
#	define LOG_HINT(str)		{logger(LogLevel::hint, str);}
#	define LOG_INFO(str)		{logger(LogLevel::normal, str);}
#else
#	define LOG_ERROR(str)		{}
#	define LOG_WARNING(str)		{}
#	define LOG_HINT(str)		{}
#	define LOG_INFO(str)		{}
#endif//LOG_OUTPUT
}//namespace kiss

#endif//KISS_LOGGER_H