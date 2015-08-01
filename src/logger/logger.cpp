#include<mutex>
#include<thread>
#include<stdio.h>
#include"logger.h"
#include<other/DateTime.h>

namespace
{
	std::mutex logger_mutex;
}

namespace kiss
{
	thread_local char thread_name[64];
	thread_local int thread_id;

	void logger(const char* text)
	{
		logger_mutex.lock();
#ifndef LOG_WITHOUT_TIME
			printf("%f ",NowTime());
#endif//LOG_WITHOUT_TIME

#ifndef LOG_WITHOUT_THREAD_NAME
			printf("%s %d %d",thread_name,thread_id,std::this_thread::get_id());
#endif//LOG_WITHOUT_THREAD_ID

			printf("%s\n",text);
		logger_mutex.unlock();
	}
}//namespace kiss