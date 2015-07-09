#ifndef KISS_TIME_H
#define KISS_TIME_H

class Date
{
public:
	Date();
	Date(const int time);

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
#endif//KISS_TIME_H