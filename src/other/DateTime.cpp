#include"DateTime.h"
#include<time.h>

#ifdef WIN32
#	include<windows.h>
#else
#	include<unistd.h>
#	include<sys/time.h>
#endif

namespace
{
	time_t boot_time_seconds;
	tm boot_time;			///<save start time

	struct TimeInit
	{
		TimeInit()
		{
			boot_time_seconds = time(nullptr);
			tm* t = localtime(&boot_time_seconds);

			boot_time.tm_year		= t->tm_year+1900;
			boot_time.tm_mon		= t->tm_mon+1;
			boot_time.tm_mday		= t->tm_mday;
			boot_time.tm_hour		= t->tm_hour;
			boot_time.tm_min		= t->tm_min;
			boot_time.tm_sec		= t->tm_sec;
			boot_time.tm_wday		= t->tm_wday?t->tm_wday:7;
			boot_time.tm_gmtoff 	= t->tm_gmtoff;
		}
	};

	TimeInit time_init;
}



namespace kiss
{
	Date::Date()
	{
		auto now_time = time(nullptr);
		auto t = localtime(&now_time);

		year	= t->tm_year+1900;
		month	= t->tm_mon+1;
		day		= t->tm_mday;
		hour	= t->tm_hour;
		minute	= t->tm_min;
		second	= t->tm_sec;
		weekday	= t->tm_wday?t->tm_wday:7;
	}

	Date::Date(const long int time)
	{
		auto t = localtime((time_t*)&time);

		year = t->tm_year + 1900;
		month = t->tm_mon + 1;
		day = t->tm_mday;
		hour = t->tm_hour;
		minute = t->tm_min;
		second = t->tm_sec;
		weekday = t->tm_wday ? t->tm_wday : 7;
	}

	int Date::Year()
	{
		return year;
	}

	int Date::Month()
	{
		return month;
	}

	int Date::Day()
	{
		return day;
	}

	int Date::Hour()
	{
		return hour;
	}

	int Date::Minute()
	{
		return minute;
	}

	int Date::Second()
	{
		return second;
	}

	int Date::Weekday()
	{
		return weekday;
	}

	Time::Time()
	{
		cur_time = time(nullptr);
	}

	Time::~Time()
	{
	}

	double NowTime()
	{
	#ifdef WIN32
			return GetLocalTime();
	#else
			struct timeval tv;
			struct timezone tz;

			gettimeofday(&tv, &tz);

			return tv.tv_sec+double(tv.tv_usec)/1000000+boot_time.tm_gmtoff;
	#endif//WIN32
	}

	long long int NowTimeMSec()
	{
	#ifdef WIN32
			return GetLocalTime();
	#else
			struct timeval tv;
			struct timezone tz;

			gettimeofday(&tv, &tz);

			return (tv.tv_sec+boot_time.tm_gmtoff)*1000+tv.tv_usec/1000;
	#endif//WIN32
	}

	long long int NowTimeUSec()
	{
	#ifdef WIN32
			return GetLocalTime();
	#else
			struct timeval tv;
			struct timezone tz;

			gettimeofday(&tv, &tz);

			return (tv.tv_sec+boot_time.tm_gmtoff)*1000000+tv.tv_usec;
	#endif//WIN32
	}

	void WaitTime(const double milliseconds)
	{
	#ifdef WIN32
		Sleep(milliseconds);
	#else
		usleep(milliseconds*1000);
	#endif
	}
}