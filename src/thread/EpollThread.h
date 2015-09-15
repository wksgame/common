#ifndef KISS_EPOLL_THREAD_H
#define KISS_EPOLL_THREAD_H

#include"Thread.h"

namespace kiss
{
	class Session;
	class EpollManage;

	class EpollThread : public Thread
	{
	public:
		EpollThread(const char* thread_name);
		~EpollThread();
		
		bool Join(Session* sock);

	protected:
		void Update();

	private:
		EpollManage* manage;
	};//class EpollThread

}//namespace kiss
#endif//KISS_EPOLL_THREAD_H