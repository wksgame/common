#include<other/DateTime.h>
#include<stdio.h>

using namespace kiss;

int main()
{
	double start=0;
	double end=0;
	start = NowTime();

//	for(int i=0; i<1000000; ++i)
		NowTime();

	end = NowTime();

	printf("%f\n",end-start);
	return 0;
}