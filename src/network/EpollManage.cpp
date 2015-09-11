#include<sys/epoll.h>
#include<unistd.h>
#include"EpollManage.h"
#include"message/Session.h"

namespace kiss
{
	EpollManage::EpollManage()
	{
		events_size = 1000;
		events = new epoll_event[events_size];
		epfd = epoll_create(events_size);
		timeout = -1;
	}
	
	EpollManage::~EpollManage()
	{
		::close(epfd);
		delete [] events;
	}
	
	bool EpollManage::Join(Session* s)
	{
		epoll_event ee;

		ee.events = EPOLLIN
//					|EPOLLOUT
					|EPOLLERR
					|EPOLLRDHUP
					|EPOLLHUP
					|EPOLLET;

		ee.data.ptr = (void*)s;

		int result = epoll_ctl(epfd,EPOLL_CTL_ADD,s->sock.GetSocketFD(),&ee);
		if(result==-1)
			return false;

		return true;
	}

	bool EpollManage::Update()
	{
		auto selret = epoll_wait(epfd, events, events_size, timeout);

		for(int i=0; i<selret; ++i)
		{
			Session* s = (Session*)events[i].data.ptr;

			if(events[i].events&(EPOLLERR|EPOLLRDHUP|EPOLLHUP))
			{
				epoll_ctl(epfd,EPOLL_CTL_DEL,s->sock.GetSocketFD(),nullptr);
				continue;
			}

			if(events[i].events&EPOLLIN)
			{
				if(!s->sock.Recv())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,s->sock.GetSocketFD(),nullptr);
					continue;
				}

				if(!s->Update())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,s->sock.GetSocketFD(),nullptr);
					continue;
				}

				if(!Process(s))
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,s->sock.GetSocketFD(),nullptr);
					continue;
				}
			}
/*			
			if(events[i].events&EPOLLOUT)
			{
				if(!sock->Send())
				{
					epoll_ctl(epfd,EPOLL_CTL_DEL,sock->Socket(),nullptr);
					sock->enable = false;
					continue;
				}
			}*/
		}

		return true;
	}
}//namespace kiss