#include<mutex>
#include<thread>
#include<stdio.h>
#include"logger.h"
#include<other/DateTime.h>

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
 * light bgcolor fgcolor
 *
 */
const char* console_color[]=
{
	"\e[01;40;41m",		// red
	"\e[01;40;43m",		// yellow
	"\e[0;40;42m",		// green
	"\e[0m"				// reset
};

namespace kiss
{
	thread_local char thread_name[64];

	void logger(const LogLevel level, const char* text)
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
#endif//LOG_WITHOUT_THREAD_ID

			printf("%s",text);

#ifndef LOG_WITHOUT_COLOR
			printf("\e[0m");
#endif//LOG_WITHOUT_COLOR

			printf("\n");

		logger_mutex.unlock();
	}
}//namespace kiss