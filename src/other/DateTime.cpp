#include"DateTime.h"
#include<time.h>

#ifdef WIN32
#	include<windows.h>
#else
#	include<unistd.h>
#endif

Date::Date()
{
	auto t = localtime(nullptr);

	year	= t->tm_year+1900;
	month	= t->tm_mon+1;
	day		= t->tm_mday;
	hour	= t->tm_hour;
	minute	= t->tm_min;
	second	= t->tm_sec;
	weekday	= t->tm_wday?t->tm_wday:7;
}

Date::Date(const int time)
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
	return time(nullptr);
}

void WaitTime(const double milisec)
{
#ifdef WIN32
	Sleep(milisec);
#else
	usleep(milisec*1000);
#endif
}