#ifndef KISS_LOGGER_H
#define KISS_LOGGER_H

namespace kiss
{
	void logger(const char* text);

#ifndef LOG_NOT_OUTPUT
#	define LOG_INFO(str)	{logger(str);}
#else
#	define LOG_INFO(str)	{}
#endif//LOG_OUTPUT
}//namespace kiss

#endif//KISS_LOGGER_H