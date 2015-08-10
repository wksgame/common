#ifndef KISS_MUTEX_H
#define KISS_MUTEX_H

#include<mutex>

template<bool enable = true>
class Mutex
{
	std::mutex _lock;

	Mutex(const Mutex&)=delete;
	Mutex& operator=(const Mutex&)=delete;

public:

	Mutex()=default;

	inline void lock(){_lock.lock();}
	inline void unlock(){_lock.unlock();}
	inline bool try_lock(){return _lock.try_lock();}
};

template<>
class Mutex<false>
{
public:
	inline void lock(){}
	inline void unlock(){}
	inline bool try_lock(){}
};
#endif//KISS_MUTEX_H