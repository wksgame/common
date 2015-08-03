#ifndef KISS_EPOLL_THREAD_H
#define KISS_EPOLL_THREAD_H

#include"Thread.h"
#include<list>
#include<mutex>
#include<platform/platform.h>
#include<sys/epoll.h>

namespace kiss
{
	class ClientSession;

	class EpollThread : public Thread
	{
	public:
		EpollThread(const char* thread_name);
		~EpollThread();
		
		void Join(ClientSession*cs);

	protected:
		void Run()override;
		void Update();

	private:
		std::list<ClientSession*>  clients;
		std::list<ClientSession*>  joinClients;
		std::list<ClientSession*>  quitClients;
		std::mutex joinLock;
		std::mutex quitLock;

		int epfd;
		epoll_event* events;
		int events_size;
		int timeout;
		
		double cur_time;
	};//class EpollThread
}//namespace kiss
#endif//KISS_EPOLL_THREAD_H