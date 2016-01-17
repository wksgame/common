#ifndef SLG_EPOLL_MANAGE_H
#define SLG_EPOLL_MANAGE_H

struct epoll_event;
#include"SocketManage.h"

namespace kiss
{
	class AcceptSocket;

	class EpollManage :public SocketManage
	{
	public:
		EpollManage(const int count);
		~EpollManage();
		
		bool Add(kiss::AcceptSocket* s)override;
		void Remove(kiss::AcceptSocket* s)override;

		bool Update()override;

	private:
		epoll_event* events;
		int epfd;
		int events_size;
		int cur_size;
		int timeout;
	};//class EpollManage
}//namespace kiss

#endif//SLG_EPOLL_MANAGE_H