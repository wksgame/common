#include<thread>
#include<stdarg.h>
#include"logger.h"
#include"other/DateTime.h"
#include"thread/Mutex.h"

#define DAY_OF_USECS	86400000000LL

namespace kiss
{
	const char* logger_level_str[]=
	{
		"[E]",
		"[W]",
		"[I]",
		"[D]",
	};

	static_assert((int)LogLevel::end == sizeof(logger_level_str)/sizeof(logger_level_str[0]),"log level string error");

	Logger syslogger("system.log");

	Logger::Logger(const char* log_file_name):file_name(log_file_name)
	{
		int64 cur_time=NowTimeUSec();
		log_day_usec = cur_time - cur_time%DAY_OF_USECS + DAY_OF_USECS;

		Date cur;
		char new_file_name[128];
		snprintf(new_file_name, 128, "%d-%02d-%02d-%s", cur.Year(), cur.Month(), cur.Day(), file_name.c_str());

		file_fd = fopen(new_file_name, "at");
	}

	Logger::~Logger()
	{
		fclose(file_fd);
	}

	void Logger::logger(const kiss::LogLevel level, const char* format, ...)
	{
		logger_mutex.lock();

			int64 cur_time = NowTimeUSec();

			if(log_day_usec<cur_time)
			{
				log_day_usec += DAY_OF_USECS;

				fclose(file_fd);

				Date log_day(int(log_day_usec/1000000));
				char new_file_name[128];
				snprintf(new_file_name,128,"%d-%02d-%02d-%s",log_day.Year(),log_day.Month(),log_day.Day(),file_name.c_str());

				file_fd = fopen(new_file_name,"at");
			}

			long int mseconds = cur_time + DAY_OF_USECS - log_day_usec;
			int hour = mseconds/3600000000;
			mseconds %= 3600000000;
			int min = mseconds/60000000;
			mseconds %= 60000000;
			int sec = mseconds/1000000;
			mseconds %= 1000000;

			fprintf(file_fd,"%s %02d:%02d:%02d.%06d ",logger_level_str[(int)level],hour,min,sec,mseconds);

			va_list args;
			va_start(args,format);
			vfprintf(file_fd,format,args);
			va_end(args);

			fprintf(file_fd,"\n");

			fflush(file_fd);

		logger_mutex.unlock();
	}

}//namespace kiss