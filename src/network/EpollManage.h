#ifndef SLG_EPOLL_MANAGE_H
#define SLG_EPOLL_MANAGE_H

#include"hgl/thread/Thread.h"

struct epoll_event;

namespace slg
{
	class Session;

	class EpollManage
	{
	public:
		EpollManage();
		~EpollManage();
		
		bool Join(Session* sock);
		bool Update();

	private:
		epoll_event* events;
		int epfd;
		int events_size;
		int cur_size;
		int timeout;
	};//class EpollManage
}//namespace slg

#endif//SLG_EPOLL_MANAGE_H