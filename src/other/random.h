#ifndef KISS_RANDOM_H
#define KISS_RANDOM_H

namespace kiss
{
	/**
	 * @return [0,RAND_MAX)
	 */
	int Random();

	/**
	* @param x >0
	* @return [0,x]
	*/
	int Random(const int x);

	/**
	 * @param x<y
	 * @return [x,y]
	 */
	int Randomxy(const int x, const int y);

	/**
	* @return [0,1)
	*/
	float RandomFloat();
}//namespace kiss
#endif//KISS_RANDOM_H