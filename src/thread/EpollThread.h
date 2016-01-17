#ifndef KISS_EPOLL_THREAD_H
#define KISS_EPOLL_THREAD_H

#include"Thread.h"
#include<list>
#include<mutex>

struct epoll_event;

namespace kiss
{
	class TCPIOSocket;

	class EpollThread : public Thread
	{
	public:
		EpollThread();
		~EpollThread();
		
		void Join(TCPIOSocket* sock);

	protected:
		void Run()override;
		void Update();

	private:
		int epfd;
		epoll_event* events;
		int events_size;
		int timeout;
	};//class EpollThread
}//namespace kiss
#endif//KISS_EPOLL_THREAD_H