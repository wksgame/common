#include<mutex>
#include<thread>
#include<stdio.h>
#include"logger.h"
#include<other/DateTime.h>
#include<stdarg.h>

namespace
{
	std::mutex logger_mutex;
}

enum class CONSOLE_COLOR
{
	RED=0,
	BULE,
};

/**
 * light1 fgcolor30-37 bgcolor40-47 
 */
const char* console_color[]=
{
	"\e[0;30;41m",		// red
	"\e[0;30;43m",		// yellow
	"\e[0;30;42m",		// green
	"\e[0m"				// reset
};

namespace kiss
{
#ifndef LOG_WITHOUT_THREAD_NAME
	thread_local char thread_name[64];
#endif//LOG_WITHOUT_THREAD_NAME

	void logger(const kiss::LogLevel level, const char* format, ...)
	{
		logger_mutex.lock();

#ifndef LOG_WITHOUT_COLOR
			printf(console_color[(int)level]);
#endif//LOG_WITHOUT_COLOR

#ifndef LOG_WITHOUT_TIME
			printf("%f ",NowTime());
#endif//LOG_WITHOUT_TIME

#ifndef LOG_WITHOUT_THREAD_NAME
			printf("%s ",thread_name);
#endif//LOG_WITHOUT_THREAD_NAME

			va_list args;
			va_start(args,format);
			vprintf(format,args);
			va_end(args);

#ifndef LOG_WITHOUT_COLOR
			printf(" \e[0m");
#endif//LOG_WITHOUT_COLOR

			printf("\n");

		logger_mutex.unlock();
	}
	
	void logger(const kiss::LogLevel level, const std::string& text)
	{
		logger(level,text.c_str());
	}
}//namespace kiss