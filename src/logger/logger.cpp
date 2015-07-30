#include<mutex>
#include<stdio.h>
#include"logger.h"

namespace
{
	std::mutex logger_mutex;
}

namespace kiss
{
	void logger(const char* text)
	{
		logger_mutex.lock();
			printf("%s\n",text);
		logger_mutex.unlock();
	}
}//namespace kiss