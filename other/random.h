#ifndef KISS_RANDOM_H
#define KISS_RANDOM_H

int Random();

/**
 * @param x x>0
 * @return [0,x]
 */
int Random(const int x);

int Randomxy(const int x, const int y);

/**
 * return [0,1)
 */
float RandomFloat();

#endif//KISS_RANDOM_H