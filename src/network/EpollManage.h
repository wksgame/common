#ifndef SLG_EPOLL_MANAGE_H
#define SLG_EPOLL_MANAGE_H

struct epoll_event;
#include"ISocketManage.h"

namespace kiss
{
	class EpollManage :public ISocketManage
	{
	public:
		EpollManage(const int count);
		~EpollManage();
		
		bool Add(AcceptSocket* s)override;
		void Remove(AcceptSocket* s)override;

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