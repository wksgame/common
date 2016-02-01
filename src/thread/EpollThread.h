#ifndef KISS_EPOLL_THREAD_H
#define KISS_EPOLL_THREAD_H

#include"SocketThread.h"

namespace kiss
{
	class AcceptSocket;
	class EpollManage;

	class EpollThread : public SocketThread
	{
		EpollThread(const EpollThread&)=delete;
		EpollThread& operator=(const EpollThread&)=delete;
		
	public:
		EpollThread();
		~EpollThread();
		
		bool Add(kiss::AcceptSocket* sock)override;
		void Remove(kiss::AcceptSocket* s)override;

	protected:
		//void Run()override;
		void Update()override;

 	private:
 		EpollManage* manage;
	};//class EpollThread

}//namespace kiss
#endif//KISS_EPOLL_THREAD_H