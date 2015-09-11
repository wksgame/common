#ifndef KISS_EPOLL_THREAD_H
#define KISS_EPOLL_THREAD_H

#include"Thread.h"
#include<list>
#include<mutex>

struct epoll_event;

namespace kiss
{
	class AcceptSocket;

	class EpollThread : public Thread
	{
	public:
		EpollThread(const char* thread_name);
		~EpollThread();
		
		void Join(AcceptSocket* sock);

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