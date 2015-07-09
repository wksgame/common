#include"random.h"
#include<stdlib.h>
#include<time.h>

int Random()
{
	return rand();
}

int Random(const int x)
{
	return rand() % x + 1;
}

int Randomxy(const int x, const int y)
{
	int i = y - x + 1;
	return rand() % i + x;
}

float RandomFloat()
{
	return (float)rand() / INT_MAX;
}