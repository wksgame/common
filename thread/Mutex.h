#ifndef KISS_MUTEX_H
#define KISS_MUTEX_H

#include<mutex>

class Mutex
{
	std::mutex lock;

public:

	void Lock()
	{
		lock.lock();
	}

	void Unlock()
	{
		lock.unlock();
	}

	bool TryLock()
	{
		return lock.try_lock();
	}
};
#endif//KISS_MUTEX_H