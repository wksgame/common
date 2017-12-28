#ifndef KISS_TIME_H
#define KISS_TIME_H

#include "defines.h"

namespace kiss
{
	class Date
	{
	public:
		Date();
		Date(const long int time);

		int Year();
		int Month();
		int Day();
		int Hour();
		int Minute();
		int Second();
		int Weekday();

	private:
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		int weekday;
	};//class Date

	class Time
	{
	public:
		Time();
		~Time();

	private:

		long long cur_time;

	};//class Time

	/**
	 * 获取以秒为单位的double时间
	 */
	double NowTime();

	/**
	 * 获取以毫秒为单位的int64时间
	 */
	int64 NowTimeMSec();

	/**
	 * 获取以微秒为单位的int64时间
	 */
	int64 NowTimeUSec();

	void WaitTime(const double milliseconds);
}//namespace kiss
#endif//KISS_TIME_H