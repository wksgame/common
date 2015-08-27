#include<thread>
#include<stdio.h>
#include"logger.h"
#include<other/DateTime.h>
#include<stdarg.h>
#include<thread/Mutex.h>

#define DAY_OF_USECS	86400000000LL

namespace kiss
{
	const char* logger_level_str[]=
	{
		"##erro##",
		"==warn==",
		"--hint--",
		"--info--",
	};

	static_assert((int)LogLevel::end == sizeof(logger_level_str)/sizeof(logger_level_str[0]),"log level string error");

	Logger syslogger("system.log");

	Logger::Logger(const char* log_file_name):file_name(log_file_name)
	{
		cur_time=NowTimeUSec();
		write_time=cur_time;
		log_day_msec = cur_time - cur_time%DAY_OF_USECS;

		file_fd = fopen(file_name,"at");
	}

	Logger::~Logger()
	{
		fclose(file_fd);
	}

	void Logger::logger(const LogLevel level, const char* format, ...)
	{
		logger_mutex.lock();

			cur_time = NowTimeUSec();

			if(log_day_msec+DAY_OF_USECS<cur_time)
			{
				fclose(file_fd);

				Date log_day(int(log_day_msec/1000000));
				char new_file_name[128];

				snprintf(new_file_name,128,"%d-%2d-%2d-%s",log_day.Year(),log_day.Month(),log_day.Day(),file_name);

				rename(file_name,new_file_name);

				log_day_msec = cur_time - cur_time%DAY_OF_USECS;

				file_fd = fopen(file_name,"at");
			}

			long int mseconds = cur_time - log_day_msec;
			int hour = mseconds/3600000000;
			mseconds %= 3600000000;
			int min = mseconds/60000000;
			mseconds %= 60000000;
			int sec = mseconds/60000000;
			mseconds %= 1000000;

			fprintf(file_fd,"%s %02d:%02d:%02d.%06d ",logger_level_str[(int)level],hour,min,sec,mseconds);

			va_list args;
			va_start(args,format);
			vfprintf(file_fd,format,args);
			va_end(args);

			fprintf(file_fd,"\n");

			if(write_time+1000000<cur_time)
			{
				fflush(file_fd);
				write_time=cur_time;
			}

		logger_mutex.unlock();
	}

	void Logger::error(const char* format, ...)
	{
		va_list args;
		va_start(args,format);
		logger(LogLevel::error,format,args);
		va_end(args);
	}

	void Logger::warn(const char* format, ...)
	{
		va_list args;
		va_start(args,format);
		logger(LogLevel::warn,format,args);
		va_end(args);
	}

	void Logger::hint(const char* format, ...)
	{
		va_list args;
		va_start(args,format);
		logger(LogLevel::hint,format,args);
		va_end(args);
	}

	void Logger::info(const char* format, ...)
	{
		va_list args;
		va_start(args,format);
		logger(LogLevel::normal,format,args);
		va_end(args);
	}

}//namespace kiss